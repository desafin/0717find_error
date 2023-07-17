#include "../Common/Common.h"



int main()
{
	// �̹��� ������ �÷��� �о�ɴϴ�.
	cv::Mat image = cv::imread("solar_cell_05.png", cv::IMREAD_COLOR);

	// �̹����� �׷��̽����Ϸ� ��ȯ�մϴ�.
	cv::Mat gray_image;
	cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

	// �ٿ�� �ڽ��� �׸� �ȼ����� ��ǥ�� ������ ���͸� �����մϴ�.
	std::vector<cv::Point> bounding_box_pixels;

	// ����ȭ�� �����մϴ�.
	cv::Mat binary_image;
	cv::threshold(gray_image, binary_image, 200, 255, cv::THRESH_BINARY);

	const int MIN_CLUSTER_SIZE = 0;  // �ּ� �ȼ� ���� ũ��

	// Morphological Dilation�� ����Ͽ� ������ ����
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::Mat dilated_image;
	cv::dilate(binary_image, dilated_image, kernel, cv::Point(-1, -1), 1);

	// ����ȭ�� �̹������� ������ �ȼ��� ã��, ���ӵ� ������ �ȼ��� 2�ȼ� �̻����� Ȯ���մϴ�.
	for (int y = 10; y < dilated_image.rows - 10; ++y)
	{
		for (int x = 10; x < dilated_image.cols - 10; ++x)
		{
			if (dilated_image.at<uchar>(y, x) == 0)  // ������ �ȼ��� ���
			{
				if (cv::countNonZero(dilated_image(cv::Rect(x - 2, y, 4, 1))) >= 2)
				{
					// �¿쿡 ��� �ȼ��� �ִ��� Ȯ���մϴ�.
					if (cv::countNonZero(dilated_image(cv::Rect(x - 10, y, 8, 1))) > 0 &&
						cv::countNonZero(dilated_image(cv::Rect(x + 4, y, 7, 1))) > 0)
					{
						// ���Ͽ� ������ �ȼ��� �ִ��� Ȯ���մϴ�.
						if (cv::countNonZero(dilated_image(cv::Rect(x, y - 10, 1, 10))) == 0 &&
							cv::countNonZero(dilated_image(cv::Rect(x, y + 1, 1, 10))) == 0)
						{
							// �ȼ� ������ ���̰� 100�ȼ� �̻��̰� ũ�Ⱑ MIN_CLUSTER_SIZE �������� Ȯ���մϴ�.
							cv::Mat column = dilated_image.col(x).rowRange(0, y);
							int cluster_height = y - cv::countNonZero(column == 255);
							if (cluster_height >= 0 && cluster_height > MIN_CLUSTER_SIZE)
							{
								bounding_box_pixels.push_back(cv::Point(x, y));
							}
						}
					}
				}
			}
		}
	}

	// �ٿ�� �ڽ��� �׷��� �̹����� �����մϴ�.
	cv::Mat bounding_box_image = dilated_image.clone();
	cv::cvtColor(bounding_box_image, bounding_box_image, cv::COLOR_GRAY2BGR);

	// ���� �׸��ϴ�.
	for (const auto& pixel : bounding_box_pixels)
	{
		int x = pixel.x;
		int y = pixel.y;
		if (x < 950)
		{
			cv::circle(image, cv::Point(x, y), 20, cv::Scalar(0, 255, 0), 2);
		}
	}

	// �ٿ�� �ڽ��� �׸��ϴ�.
	for (const auto& pixel : bounding_box_pixels)
	{
		int x = pixel.x;
		int y = pixel.y;
		if (x < 960)  // x ��ǥ�� 960 �̸��� ��쿡�� �ٿ�� �ڽ��� �׸��ϴ�.
		{
			cv::rectangle(image, cv::Point(x - 1, y - 1), cv::Point(x + 1, y + 1), cv::Scalar(0, 0, 255), 1);
		}
	}

	// �ٿ�� �ڽ��� ���� �׷��� �̹����� ǥ���մϴ�.
	cv::imshow("result Image", image);
	cv::imshow("Contour Image", bounding_box_image);
	cv::imshow("dilated_image", dilated_image);

	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}
