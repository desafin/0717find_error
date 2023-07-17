import cv2
import numpy as np

# 이미지 파일을 컬러로 읽어옵니다.
image = cv2.imread('solar_cell_03.png', cv2.IMREAD_COLOR)

# 이미지를 그레이스케일로 변환합니다.
gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

# 바운딩 박스를 그릴 픽셀들의 좌표를 저장할 리스트를 생성합니다.
bounding_box_pixels = []

# 이진화를 수행합니다.
_, binary_image = cv2.threshold(gray_image, 200, 255, cv2.THRESH_BINARY)

# Morphological Dilation을 사용하여 노이즈 제거
kernel = np.ones((3, 3), np.uint8)
dilated_image = cv2.dilate(binary_image, kernel, iterations=10)
cv2.imshow("Dilated Image", dilated_image)

# 이진화된 이미지에서 검은색 픽셀을 찾고, 연속된 검은색 픽셀이 2픽셀 이상이고 흰색 픽셀이 5개 이상인지 확인합니다.
for y in range(10, dilated_image.shape[0] - 10):
    for x in range(10, dilated_image.shape[1] - 10):
        if dilated_image[y, x] == 0:  # 검은색 픽셀인 경우
            if np.sum(dilated_image[y, x-2:x+2] == 0) >= 2:
                # 좌우에 흰색 픽셀이 있는지 확인합니다.
                if np.sum(dilated_image[y, x-10:x-2] == 255) >= 100 and np.sum(dilated_image[y, x+4:x+11] == 255) >= 100:
                    # 상하에 흰색 픽셀이 존재하지 않으면 바운딩 박스를 그립니다.
                    if not np.any(dilated_image[y-10:y, x] == 255) and not np.any(dilated_image[y+1:y+11, x] == 255):
                        bounding_box_pixels.append((x, y))

# 바운딩 박스를 그립니다.
bounding_box_image = cv2.cvtColor(dilated_image, cv2.COLOR_GRAY2BGR)
for pixel in bounding_box_pixels:
    x, y = pixel
    cv2.rectangle(bounding_box_image, (x-1, y-1), (x+1, y+1), (0, 0, 255), 1)

# 바운딩 박스가 그려진 이미지를 표시합니다.
cv2.imshow("Contour Image", bounding_box_image)

cv2.waitKey(0)
cv2.destroyAllWindows()
