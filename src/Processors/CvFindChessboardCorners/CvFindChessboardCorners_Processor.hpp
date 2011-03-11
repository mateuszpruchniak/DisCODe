/*!
 * \file CvFindChessboardCorners_Processor.hpp
 * \brief Chessboard localization component.
 * \date Oct 20, 2010
 * \author mboryn
 */

#ifndef CVFINDCHESSBOARDCORNERS_PROCESSOR_HPP_
#define CVFINDCHESSBOARDCORNERS_PROCESSOR_HPP_

#include <cv.h>
#include <boost/shared_ptr.hpp>
#include "Component_Aux.hpp"
#include "Panel_Empty.hpp"
#include "Objects3D/Chessboard.hpp"
#include "Drawable.hpp"
#include "Timer.hpp"

#include "Property.hpp"


/**
 * \defgroup CvFindChessboardCorners CvFindChessboardCorners
 * \ingroup Processors
 *
 * \brief Locates chessboard on the image.
 *
 * \par Data streams:
 *
 * \streamin{in_img,cv::Mat}
 * Input image.
 * \streamout{out_chessboard,Types::Objects3D::Chessboard}
 * Located chessboard.
 *
 * \par Events:
 *
 * \event{chessboardFound}
 * Chessboard has been found.
 *
 * \event{chessboardNotFound}
 * Chessboard has not been found.
 *
 *
 *
 * \par Event handlers:
 *
 * \handler{onNewImage}
 * New image arrived
 *
 * \par Properties:
 * \prop{width,int,""}
 * Chessboard width.
 * This is number of corners, not the number of fields. Number of corners = number of fields - 1.
 *
 * \prop{height,int,""}
 * Chessboard height.
 * This is number of corners, not the number of fields. Number of corners = number of fields - 1.
 *
 * \prop{squareSize,int,""}
 * Square size in meters.
 *
 * \see http://opencv.willowgarage.com/documentation/cpp/camera_calibration_and_3d_reconstruction.html#cv-findchessboardcorners
 * @{
 *
 * @}
 */


namespace Processors {
namespace CvFindChessboardCorners {

struct CvFindChessboardCornersProps: public Base::Props
{
	cv::Size patternSize;
	float squareSize;
	bool findSubpix;
	bool fastCheck;
	bool filterQuads;
	bool adaptiveThreshold;
	bool normalizeImage;
	void load(const ptree & pt)
	{
		LOG(LTRACE) << "CvFindChessboardCornersProps::load()\n";
		//patternSize.width = pt.get<int>("width");
		//patternSize.height = pt.get<int>("height");
		squareSize = pt.get<float>("squareSize");
		findSubpix = pt.get<bool>("findSubpix");
		fastCheck = pt.get<bool>("fastCheck");
		filterQuads = pt.get<bool>("filterQuads");
		adaptiveThreshold = pt.get<bool>("adaptiveThreshold");
		normalizeImage = pt.get<bool>("normalizeImage");
	}
	void save(ptree & pt)
	{
		LOG(LTRACE) << "CvFindChessboardCornersProps::save()\n";
		pt.put("width", patternSize.width);
		pt.put("height", patternSize.height);
		pt.put("squareSize", squareSize);
		pt.put("findSubpix", findSubpix);
		pt.put("fastCheck", fastCheck);
		pt.put("filterQuads", filterQuads);
		pt.put("adaptiveThreshold", adaptiveThreshold);
		pt.put("normalizeImage", normalizeImage);
	}
};

class CvFindChessboardCorners_Processor: public Base::Component
{
public:
	CvFindChessboardCorners_Processor(const std::string & name = "");
	virtual ~CvFindChessboardCorners_Processor();

	Base::Props * getProperties()
	{
		return &props;
	}
protected:
	/*!
	 * Method called when component is started
	 * \return true on success
	 */
	virtual bool onStart();

	/*!
	 * Method called when component is stopped
	 * \return true on success
	 */
	virtual bool onStop();

	/*!
	 * Method called when component is initialized
	 * \return true on success
	 */
	virtual bool onInit();

	/*!
	 * Method called when component is finished
	 * \return true on success
	 */
	virtual bool onFinish();

	/*!
	 * Method called when step is called
	 * \return true on success
	 */
	virtual bool onStep();


private:
	void onNewImage();

	void initChessboard();

	/** New image event handler. */
	Base::EventHandler <CvFindChessboardCorners_Processor> h_onNewImage;
	/** Image stream. */
	Base::DataStreamIn <cv::Mat> in_img;
	/** Chessboard stream. */
	Base::DataStreamOut <Types::Objects3D::Chessboard> out_chessboard;
	/** Raised when chessboard has been located on the image. */
	Base::Event *chessboardFound;
	/** Raised when chessboard has not been located on the image. */
	Base::Event *chessboardNotFound;

	/** Located corners.*/
	std::vector<cv::Point2f> corners;

	int findChessboardCornersFlags;

	Common::Timer timer;

	CvFindChessboardCornersProps props;

	boost::shared_ptr<Types::Objects3D::Chessboard> chessboard;

	cv::Mat sub_img;

	Base::Property<bool> prop_subpix;
	Base::Property<int> prop_subpix_window;
	Base::Property<bool> prop_scale;
	Base::Property<int> prop_scale_factor;
	Base::Property<int> prop_width;
	Base::Property<int> prop_height;
	Base::Property<float> prop_square_width;
	Base::Property<float> prop_square_height;

	Base::Property<bool> prop_fastCheck;
	Base::Property<bool> prop_filterQuads;
	Base::Property<bool> prop_adaptiveThreshold;
	Base::Property<bool> prop_normalizeImage;


	void sizeCallback(int old_value, int new_value);
	void flagsCallback(bool old_value, bool new_value);
};

} // namespace CvFindChessboardCorners {

} // namespace Processors {

REGISTER_PROCESSOR_COMPONENT("CvFindChessboardCorners", Processors::CvFindChessboardCorners::CvFindChessboardCorners_Processor, Common::Panel_Empty)

#endif /* CVFINDCHESSBOARDCORNERS_PROCESSOR_HPP_ */
