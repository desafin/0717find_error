#include "../Common/Common.h"



int main()
{
	// 이미지 파일을 컬러로 읽어옵니다.
	cv::Mat image = cv::imread("solar_cell_05.png", cv::IMREAD_COLOR);

	// 이미지를 그레이스케일로 변환합니다.
	cv::Mat gray_image;
	cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

	// 바운딩 박스를 그릴 픽셀들의 좌표를 저장할 벡터를 생성합니다.
	std::vector<cv::Point> bounding_box_pixels;

	// 이진화를 수행합니다.
	cv::Mat binary_image;
	cv::threshold(gray_image, binary_image, 200, 255, cv::THRESH_BINARY);

	const int MIN_CLUSTER_SIZE = 0;  // 최소 픽셀 군집 크기

	// Morphological Dilation을 사용하여 노이즈 제거
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::Mat dilated_image;
	cv::dilate(binary_image, dilated_image, kernel, cv::Point(-1, -1), 1);

	// 이진화된 이미지에서 검은색 픽셀을 찾고, 연속된 검은색 픽셀이 2픽셀 이상인지 확인합니다.
	for (int y = 10; y < dilated_image.rows - 10; ++y)
	{
		for (int x = 10; x < dilated_image.cols - 10; ++x)
		{
			if (dilated_image.at<uchar>(y, x) == 0)  // 검은색 픽셀인 경우
			{
				if (cv::countNonZero(dilated_image(cv::Rect(x - 2, y, 4, 1))) >= 2)
				{
					// 좌우에 흰색 픽셀이 있는지 확인합니다.
					if (cv::countNonZero(dilated_image(cv::Rect(x - 10, y, 8, 1))) > 0 &&
						cv::countNonZero(dilated_image(cv::Rect(x + 4, y, 7, 1))) > 0)
					{
						// 상하에 검은색 픽셀이 있는지 확인합니다.
						if (cv::countNonZero(dilated_image(cv::Rect(x, y - 10, 1, 10))) == 0 &&
							cv::countNonZero(dilated_image(cv::Rect(x, y + 1, 1, 10))) == 0)
						{
							// 픽셀 군집의 높이가 100픽셀 이상이고 크기가 MIN_CLUSTER_SIZE 이하인지 확인합니다.
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

	// 바운딩 박스가 그려진 이미지를 생성합니다.
	cv::Mat bounding_box_image = dilated_image.clone();
	cv::cvtColor(bounding_box_image, bounding_box_image, cv::COLOR_GRAY2BGR);

	// 원을 그립니다.
	for (const auto& pixel : bounding_box_pixels)
	{
		int x = pixel.x;
		int y = pixel.y;
		if (x < 950)
		{
			cv::circle(image, cv::Point(x, y), 20, cv::Scalar(0, 255, 0), 2);
		}
	}

	// 바운딩 박스를 그립니다.
	for (const auto& pixel : bounding_box_pixels)
	{
		int x = pixel.x;
		int y = pixel.y;
		if (x < 960)  // x 좌표가 960 미만인 경우에만 바운딩 박스를 그립니다.
		{
			cv::rectangle(image, cv::Point(x - 1, y - 1), cv::Point(x + 1, y + 1), cv::Scalar(0, 0, 255), 1);
		}
	}

	// 바운딩 박스와 원이 그려진 이미지를 표시합니다.
	cv::imshow("result Image", image);
	cv::imshow("Contour Image", bounding_box_image);
	cv::imshow("dilated_image", dilated_image);

	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}
