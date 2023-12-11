import cv2

def Fast(ipath,opath):
    img = cv2.imread(ipath)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    fast = cv2.FastFeatureDetector_create(40)

    kp = fast.detect(gray,None)
    img2 = cv2.drawKeypoints(img, kp, None)
    height, width = img.shape[:2]
    scale_factor_x = 10 / width
    scale_factor_y = 10 / height

    for point in kp:
        point.pt = (point.pt[0] * scale_factor_x, point.pt[1] * scale_factor_y)

    print("Threshold: ", fast.getThreshold())
    print("nonmaxSuppression: ", fast.getNonmaxSuppression())
    print("neighborhood: ", fast.getType())
    print("Total Keypoints with nonmaxSuppression: ", kp[0].pt)

    with open(opath, 'w') as file:
        for point in kp:
            x, y = point.pt
            file.write(f'{x} {y}\n')
    # display the image with keypoints drawn on it
    cv2.imshow("Keypoints with nonmaxSuppression", img2)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

Fast('Feature_Matching_DT\Fast_Detection\input1.jpeg','Feature_Matching_DT\Delaunay_Triangulation\keypoints1.txt') 
Fast('Feature_Matching_DT\Fast_Detection\input2.jpeg','Feature_Matching_DT\Delaunay_Triangulation\keypoints2.txt')
