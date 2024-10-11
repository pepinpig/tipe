import cv2
import numpy as np
import matplotlib.pyplot as plt

# Function to show images side by side
def show_images_side_by_side(image1, image2, title1=None, title2=None):
    fig, axs = plt.subplots(1, 2, figsize=(12, 6))
    
    # Show the first image
    axs[0].imshow(cv2.cvtColor(image1, cv2.COLOR_BGR2RGB))  # Convert BGR to RGB for visualization
    if title1:
        axs[0].set_title(title1)
    axs[0].axis('off')  # Hide axis

    # Show the second image
    axs[1].imshow(cv2.cvtColor(image2, cv2.COLOR_BGR2RGB))  # Convert BGR to RGB for visualization
    if title2:
        axs[1].set_title(title2)
    axs[1].axis('off')  # Hide axis

    plt.show()

# Detect keypoints and compute descriptors using ORB
def detect_and_compute(image):
    orb = cv2.ORB_create(nfeatures=500)  # ORB detector
    keypoints, descriptors = orb.detectAndCompute(image, None)
    return keypoints, descriptors

# Match keypoints between two images using FLANN-based matcher
def match_keypoints(descriptors1, descriptors2):
    # FLANN parameters
    index_params = dict(algorithm=6,  # LSH algorithm for ORB descriptors
                        table_number=6,  # 12
                        key_size=12,     # 20
                        multi_probe_level=1)  # 2
    search_params = dict(checks=50)  # or pass an empty dictionary

    flann = cv2.FlannBasedMatcher(index_params, search_params)
    matches = flann.knnMatch(descriptors1, descriptors2, k=2)
    
    # Filter for good matches using Lowe's ratio test
    good_matches = []
    for m, n in matches:
        if len(matches) >= 2 and m.distance < 0.7 * n.distance:
            good_matches.append(m)
    
    return good_matches

# Compute homography using RANSAC and filter out outliers
def compute_homography(keypoints1, keypoints2, good_matches):
    pts1 = np.float32([keypoints1[m.queryIdx].pt for m in good_matches]).reshape(-1, 1, 2)
    pts2 = np.float32([keypoints2[m.trainIdx].pt for m in good_matches]).reshape(-1, 1, 2)

    homography_matrix, mask = cv2.findHomography(pts1, pts2, cv2.RANSAC, 5.0)
    
    return homography_matrix, mask

# Apply homography matrix to the second image (inverse homography to align image2 with image1)
def apply_inverse_homography(image, homography_matrix, shape):
    # Apply the inverse of the homography matrix
    inverse_homography_matrix = np.linalg.inv(homography_matrix)
    warped_image = cv2.warpPerspective(image, inverse_homography_matrix, shape)
    return warped_image

# Main function to display original and transformed images
def display_original_and_transformed(image1, image2):
    # Detect keypoints and compute descriptors in both images
    keypoints1, descriptors1 = detect_and_compute(image1)
    keypoints2, descriptors2 = detect_and_compute(image2)

    # Match keypoints between the two images
    good_matches = match_keypoints(descriptors1, descriptors2)

    # Compute the homography matrix using RANSAC
    homography_matrix, mask = compute_homography(keypoints1, keypoints2, good_matches)

    # Apply the inverse homography matrix to the second image
    height, width = image1.shape[:2]
    transformed_image2 = apply_inverse_homography(image2, homography_matrix, (width, height))

    # Display the original and transformed images side by side
    show_images_side_by_side(image1, transformed_image2, title1='Original Image', title2='Transformed Image')

# Load images
image1 = cv2.imread('images/image1.jpg')
image2 = cv2.imread('images/image2.jpg')

# Display the original and transformed images side by side
display_original_and_transformed(image1, image2)
