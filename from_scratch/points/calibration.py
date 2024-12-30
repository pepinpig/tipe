import numpy as np
import cv2 as cv
import glob

# Termination criteria
criteria = (cv.TERM_CRITERIA_EPS + cv.TERM_CRITERIA_MAX_ITER, 30, 0.001)

# Prepare object points, like (0,0,0), (1,0,0), (2,0,0) ...., (6,5,0)
objp = np.zeros((6 * 7, 3), np.float32)
objp[:, :2] = np.mgrid[0:7, 0:6].T.reshape(-1, 2)

# Arrays to store object points and image points from all the images
objpoints = []  # 3D points in real world space
imgpoints = []  # 2D points in image plane

# Load images
images = glob.glob('chess_img/*.jpg')
print(f"Found {len(images)} images.")

# Resize scale (0.5 means 50% of the original size)
resize_scale = 1

for fname in images:
    img = cv.imread(fname)
    if img is None:
        print(f"Warning: Unable to load image {fname}. Skipping...")
        continue

    # Resize the image
    height, width = img.shape[:2]
    img = cv.resize(img, (int(width * resize_scale), int(height * resize_scale)))
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

    # Find the chessboard corners
    ret, corners = cv.findChessboardCorners(gray, (7, 6), None)

    # If found, add object points and image points (after refining them)
    if ret:
        objpoints.append(objp)
        corners2 = cv.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)
        imgpoints.append(corners2)

        # Draw and display the corners
        cv.drawChessboardCorners(img, (7, 6), corners2, ret)
        cv.imshow('img', img)
        print(f"Found corners in image: {fname}")
        cv.waitKey(500)
    else:
        print(f"No corners found in image: {fname}")

cv.destroyAllWindows()

# Perform camera calibration if points are available
if objpoints and imgpoints:
    ret, mtx, dist, rvecs, tvecs = cv.calibrateCamera(objpoints, imgpoints, gray.shape[::-1], None, None)

    if ret:
        print("Camera calibration successful.")
        print("Camera matrix:")
        print(mtx)
    else:
        print("Calibration failed.")
else:
    print("No valid object or image points found. Calibration aborted.")
