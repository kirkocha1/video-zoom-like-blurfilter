#include <opencv2/opencv.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudafilters.hpp>
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>
#include <stdexcept>
#include <tuple>
#include <iostream>

class VideoProcessor {
protected:
    cv::VideoCapture cap;
    cv::VideoWriter writer;
    cv::Size outputSize;

public:
    VideoProcessor(const std::string& inputPath, const std::string& outputPath) {
        setupVideo(inputPath, outputPath);
    }

    virtual void process() = 0; // Pure virtual function for processing

protected:
    void setupVideo(const std::string& inputPath, const std::string& outputPath) {
        std::cout << cv::getBuildInformation() << std::endl;

        cap.open(inputPath);
        if (!cap.isOpened()) {
            std::cerr << "Error opening video file" << std::endl;
            throw std::invalid_argument("wrong input video params");
        }

        outputSize = cv::Size(640, 360);
        writer.open(outputPath, cv::VideoWriter::fourcc('m', 'p', '4', 'v'), cap.get(cv::CAP_PROP_FPS), outputSize);

        if (!writer.isOpened()) {
            std::cerr << "Error opening video writer" << std::endl;
            throw std::invalid_argument("wrong writer params");
        }
    }
};

class DlibBlurProcessor : public VideoProcessor {
public:
    DlibBlurProcessor(const std::string& inputPath, const std::string& outputPath)
        : VideoProcessor(inputPath, outputPath) {}

    void process() override {
        dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
        cv::Mat frame;
        int frameCount = 0;
        int frameSkip = 2;

        std::cout << "Starting reading of video" << std::endl;
        while (cap.read(frame)) {
            std::cout << "Frame is read" << std::endl;
            cv::Mat resizedFrame;
            cv::resize(frame, resizedFrame, outputSize);

            dlib::cv_image<dlib::bgr_pixel> cimg(resizedFrame);
            std::vector<dlib::rectangle> faces = detector(cimg);

            cv::Mat blurredFrame;
            cv::GaussianBlur(resizedFrame, blurredFrame, cv::Size(21, 21), 0);
            cv::Mat finalFrame = resizedFrame.clone();

            if (!faces.empty()) {
                std::cout << "Face is not empty" << std::endl;
                cv::Mat mask = cv::Mat::zeros(resizedFrame.size(), CV_8UC1);
                for (const auto& face : faces) {
                    cv::rectangle(mask, cv::Rect(face.left(), face.top(), face.width(), face.height()), cv::Scalar(255), -1);
                }
                blurredFrame.copyTo(finalFrame, mask);
                resizedFrame.copyTo(finalFrame, ~mask);
            } else {
                std::cout << "Face is empty" << std::endl;
                finalFrame = blurredFrame;
            }

            cv::Mat outputFrame;
            cv::resize(finalFrame, outputFrame, frame.size(), 0, 0, cv::INTER_LINEAR);

            if (frameCount % frameSkip == 0) {
                std::cout << "Write frame to video" << std::endl;
                writer.write(outputFrame);
            }
            frameCount++;
        }
    }
};

class CaffeModelProcessor : public VideoProcessor {
public:
    CaffeModelProcessor(const std::string& inputPath, const std::string& outputPath)
        : VideoProcessor(inputPath, outputPath) {}

    void process() override {
        cv::dnn::Net faceDetector = cv::dnn::readNetFromCaffe("data/deploy.prototxt.txt", "data/res10_300x300_ssd_iter_140000.caffemodel");
        if (faceDetector.empty()) {
            std::cerr << "Error loading Caffe model" << std::endl;
            throw std::invalid_argument("Caffe model loading failed");
        }

        cv::Mat frame;
        int frameCount = 0;
        int frameSkip = 2;

        std::cout << "Starting reading of video" << std::endl;
        while (cap.read(frame)) {
            std::cout << "Frame is read" << std::endl;
            cv::Mat resizedFrame;
            cv::resize(frame, resizedFrame, outputSize);

            cv::Mat blob = cv::dnn::blobFromImage(resizedFrame, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0), false, false);
            faceDetector.setInput(blob);
            cv::Mat detections = faceDetector.forward();

            std::vector<cv::Rect> faces;
            for (int i = 0; i < detections.size[2]; i++) {
                int idx[] = {0, 0, i, 2};
                float confidence = detections.at<float>(idx);
                if (confidence > 0.5) {
                    int x1_idx[] = {0, 0, i, 3};
                    int x2_idx[] = {0, 0, i, 5};
                    int y1_idx[] = {0, 0, i, 4};
                    int y2_idx[] = {0, 0, i, 6};

                    int x1 = static_cast<int>(detections.at<float>(x1_idx) * resizedFrame.cols);
                    int y1 = static_cast<int>(detections.at<float>(y1_idx) * resizedFrame.rows);
                    int x2 = static_cast<int>(detections.at<float>(x2_idx) * resizedFrame.cols);
                    int y2 = static_cast<int>(detections.at<float>(y2_idx) * resizedFrame.rows);

                    faces.emplace_back(x1, y1, x2 - x1, y2 - y1);
                    std::cout << "Detected face at: [" << x1 << ", " << y1 << "] [" << x2 << ", " << y2 << "]" << std::endl;
                }
            }

            cv::Mat blurredFrame;
            cv::GaussianBlur(resizedFrame, blurredFrame, cv::Size(45, 45), 0);

            for (const auto& face : faces) {
                cv::Rect boundedFace = face & cv::Rect(0, 0, resizedFrame.cols, resizedFrame.rows);
                resizedFrame(boundedFace).copyTo(blurredFrame(boundedFace));
            }

            if (frameCount % frameSkip == 0) {
                std::cout << "Write frame to video" << std::endl;
                writer.write(blurredFrame);
            }
            frameCount++;
        }
    }
};

class HaarCascadeProcessor : public VideoProcessor {
public:
    HaarCascadeProcessor(const std::string& inputPath, const std::string& outputPath)
        : VideoProcessor(inputPath, outputPath) {}

    void process() override {
        cv::CascadeClassifier faceDetector("data/haarcascade_frontalface_default.xml");
        if (faceDetector.empty()) {
            std::cerr << "Error loading Haar Cascade model" << std::endl;
            throw std::invalid_argument("Haar Cascade model loading failed");
        }

        cv::Mat frame;
        int frameCount = 0;
        int frameSkip = 2;

        std::cout << "Starting reading of video" << std::endl;
        while (cap.read(frame)) {
            std::cout << "Frame is read" << std::endl;
            cv::Mat resizedFrame;
            cv::resize(frame, resizedFrame, outputSize);

            std::vector<cv::Rect> faces;
            faceDetector.detectMultiScale(resizedFrame, faces, 1.1, 2, 0, cv::Size(30, 30));

            cv::Mat blurredFrame;
            cv::GaussianBlur(resizedFrame, blurredFrame, cv::Size(21, 21), 0);
            cv::Mat finalFrame = resizedFrame.clone();

            if (!faces.empty()) {
                std::cout << "Face is not empty" << std::endl;
                cv::Mat mask = cv::Mat::zeros(resizedFrame.size(), CV_8UC1);
                for (const auto& face : faces) {
                    cv::rectangle(mask, face, cv::Scalar(255), -1);
                }
                blurredFrame.copyTo(finalFrame, mask);
                resizedFrame.copyTo(finalFrame, ~mask);
            } else {
                std::cout << "Face is empty" << std::endl;
                finalFrame = blurredFrame;
            }

            cv::Mat outputFrame;
            cv::resize(finalFrame, outputFrame, frame.size(), 0, 0, cv::INTER_LINEAR);

            if (frameCount % frameSkip == 0) {
                std::cout << "Write frame to video" << std::endl;
                writer.write(outputFrame);
            }
            frameCount++;
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <dlib|caffe|haarcascade> <input_video> <output_video>" << std::endl;
        return -1;
    }

    std::string method = argv[1];
    std::string inputPath = argv[2];
    std::string outputPath = argv[3];

    try {
        if (method == "dlib") {
            DlibBlurProcessor processor(inputPath, outputPath);
            processor.process();
        } else if (method == "caffe") {
            CaffeModelProcessor processor(inputPath, outputPath);
            processor.process();
        } else if (method == "haarcascade") {
            HaarCascadeProcessor processor(inputPath, outputPath);
            processor.process();
        } else {
            std::cerr << "Invalid method. Use 'dlib', 'caffe', or 'haarcascade'." << std::endl;
            return -1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
