#include "../Common/Common.h"

int main()
{
    // �̹��� ������ �÷��� �о�ɴϴ�.
    cv::Mat image = cv::imread("brake_disk_part_01.png", cv::IMREAD_COLOR);

    // �̹����� �׷��̽����Ϸ� ��ȯ�մϴ�.
    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

    cv::imshow("gray_image", gray_image);

    // ����ȭ�� ���� �Ӱ谪�� �����մϴ�.
    int threshold_value = 254;
    int max_value = 255;

    // ����ȭ�� �����մϴ�.
    cv::Mat binary_image;
    cv::threshold(gray_image, binary_image, threshold_value, max_value, cv::THRESH_BINARY);

    // ����ȭ�� �̹����� ����մϴ�.
    cv::imshow("binary_image", binary_image);

    // ����� ã���ϴ�.
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary_image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // ����� ���������� �׸��ϴ�.
    cv::drawContours(image, contours, -1, cv::Scalar(0, 0, 255), 2);

    // ���� ���� �׸��� �ε����� �������� ����մϴ�.
    for (size_t i = 0; i < contours.size(); i++) {
        if (cv::contourArea(contours[i]) < 100) {
            continue; // ������ 100���� ������ �ǳʶݴϴ�.
        }

        cv::Point2f center;
        float radius;
        cv::minEnclosingCircle(contours[i], center, radius);

        // ���� �� �׸���
        cv::circle(image, center, static_cast<int>(radius), cv::Scalar(0, 255, 0), 2);

        // �ε����� ������ ���
        std::string text = "Index: " + std::to_string(i) + ", Radius: " + std::to_string(radius);
        cv::putText(image, text, cv::Point(center.x - 50, center.y - 20), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
    }

    // ������� ���� ���� �׷��� �̹����� ����մϴ�.
    cv::imshow("image", image);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
