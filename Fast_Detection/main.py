import cv2

# read input image
img = cv2.imread('Fast_Detection\input.png')

# convert the image to grayscale
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# Initiate FAST object with default values
fast = cv2.FastFeatureDetector_create(150)

# find the keypoints on image (grayscale)
kp = fast.detect(gray,None)

# draw keypoints in image
img2 = cv2.drawKeypoints(img, kp, None)

# Print all default params
print("Threshold: ", fast.getThreshold())
print("nonmaxSuppression: ", fast.getNonmaxSuppression())
print("neighborhood: ", fast.getType())
print("Total Keypoints with nonmaxSuppression: ", kp[0].pt)

with open('keypoints.txt', 'w') as file:
    for point in kp:
        x, y = point.pt
        file.write(f'{x} {y}\n')
# display the image with keypoints drawn on it
cv2.imshow("Keypoints with nonmaxSuppression", img2)
cv2.waitKey(0)
cv2.destroyAllWindows()