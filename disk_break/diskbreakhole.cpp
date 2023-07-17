#include "../Common/Common.h"

int main()
{
    // 이미지 파일을 컬러로 읽어옵니다.
    cv::Mat image = cv::imread("brake_disk_part_01.png", cv::IMREAD_COLOR);

    // 이미지를 그레이스케일로 변환합니다.
    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

    cv::imshow("gray_image", gray_image);

    // 이진화를 위한 임계값을 설정합니다.
    int threshold_value = 254;
    int max_value = 255;

    // 이진화를 수행합니다.
    cv::Mat binary_image;
    cv::threshold(gray_image, binary_image, threshold_value, max_value, cv::THRESH_BINARY);

    // 이진화된 이미지를 출력합니다.
    cv::imshow("binary_image", binary_image);

    // 컨투어를 찾습니다.
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary_image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 컨투어를 빨간색으로 그립니다.
    cv::drawContours(image, contours, -1, cv::Scalar(0, 0, 255), 2);

    // 외접 원을 그리고 인덱스와 반지름을 출력합니다.
    for (size_t i = 0; i < contours.size(); i++) {
        if (cv::contourArea(contours[i]) < 100) {
            continue; // 면적이 100보다 작으면 건너뜁니다.
        }

        cv::Point2f center;
        float radius;
        cv::minEnclosingCircle(contours[i], center, radius);

        // 외접 원 그리기
        cv::circle(image, center, static_cast<int>(radius), cv::Scalar(0, 255, 0), 2);

        // 인덱스와 반지름 출력
        std::string text = "Index: " + std::to_string(i) + ", Radius: " + std::to_string(radius);
        cv::putText(image, text, cv::Point(center.x - 50, center.y - 20), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
    }

    // 컨투어와 외접 원이 그려진 이미지를 출력합니다.
    cv::imshow("image", image);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
