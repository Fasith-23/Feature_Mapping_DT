import cv2

# read input image
img = cv2.imread('Feature_Matching_DT\Fast_Detection\input2.jpeg')

# convert the image to grayscale
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# Initiate FAST object with default values
fast = cv2.FastFeatureDetector_create(40)

# find the keypoints on image (grayscale)
kp = fast.detect(gray,None)
img2 = cv2.drawKeypoints(img, kp, None)
height, width = img.shape[:2]
scale_factor_x = 10 / width
scale_factor_y = 10 / height
# Scale down keypoints to fit within 100x100 grid

for point in kp:
    point.pt = (point.pt[0] * scale_factor_x, point.pt[1] * scale_factor_y)
# draw keypoints in image


# Print all default params
print("Threshold: ", fast.getThreshold())
print("nonmaxSuppression: ", fast.getNonmaxSuppression())
print("neighborhood: ", fast.getType())
print("Total Keypoints with nonmaxSuppression: ", kp[0].pt)

with open('Feature_Matching_DT\keypoints.txt', 'w') as file:
    for point in kp:
        x, y = point.pt
        file.write(f'{x} {y}\n')
# display the image with keypoints drawn on it
cv2.imshow("Keypoints with nonmaxSuppression", img2)
cv2.waitKey(0)
cv2.destroyAllWindows()