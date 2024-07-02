#include <opencv2/opencv.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudafilters.hpp>
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>

int main() {
    cv::VideoCapture cap("input_video.mp4");
    if (!cap.isOpened()) {
        std::cerr << "Error opening video file" << std::endl;
        return -1;
    }

    cv::VideoWriter writer("output_video.mp4", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 
                           cap.get(cv::CAP_PROP_FPS), 
                           cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)));

    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
    dlib::shape_predictor pose_model;
    dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;

    cv::Mat frame;
    while (cap.read(frame)) {
        // Face detection
        dlib::cv_image<dlib::bgr_pixel> cimg(frame);
        std::vector<dlib::rectangle> faces = detector(cimg);

        // Create mask
        cv::Mat mask = cv::Mat::zeros(frame.size(), CV_8UC1);
        for (auto face : faces) {
            cv::rectangle(mask, cv::Rect(face.left(), face.top(), face.width(), face.height()), cv::Scalar(255), -1);
        }

        // Apply blurring with CUDA
        cv::cuda::GpuMat d_frame, d_mask, d_blurred;
        d_frame.upload(frame);
        d_mask.upload(mask);

        cv::cuda::GpuMat d_blur;
        cv::Ptr<cv::cuda::Filter> filter = cv::cuda::createGaussianFilter(d_frame.type(), d_frame.type(), cv::Size(15, 15), 0);
        filter->apply(d_frame, d_blur);

        d_blur.copyTo(d_frame, d_mask);
        d_frame.download(frame);

        // Write frame to video
        writer.write(frame);
    }

    cap.release();
    writer.release();
    return 0;
}
