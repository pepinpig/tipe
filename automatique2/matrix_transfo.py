# Import required libraries
import matplotlib.pyplot as plt
import numpy as np
import cv2
import matplotlib
matplotlib.use('TkAgg')  # Ou 'Qt5Agg'


# Function to show two images side by side
def show_images_side_by_side(image1, image2, title1=None, title2=None, cmap1=None, cmap2=None):
    fig, axs = plt.subplots(1, 2, figsize=(12, 6))
    
    # Show the first image
    axs[0].imshow(image1, cmap=cmap1)
    if title1:
        axs[0].set_title(title1)
    axs[0].axis('off')  # Hide axis

    # Show the second image
    axs[1].imshow(image2, cmap=cmap2)
    if title2:
        axs[1].set_title(title2)
    axs[1].axis('off')  # Hide axis

    plt.show()

# Function to find keypoints and descriptors using ORB
def detect_and_compute(image):
    orb = cv2.ORB_create(nfeatures=500)  # ORB detector with 500 key points
    keypoints, descriptors = orb.detectAndCompute(image, None)
    return keypoints, descriptors

# Function to match keypoints between two images using Brute Force Matcher and RANSAC
def match_keypoints_and_find_homography(image1, image2, keypoints1, keypoints2, descriptors1, descriptors2):
    # Brute Force Matcher with Hamming distance (since ORB uses binary descriptors)
    bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)
    
    # Match descriptors
    matches = bf.match(descriptors1, descriptors2)
    
    # Sort matches by distance (best matches first)
    matches = sorted(matches, key=lambda x: x.distance)
    
    # Extract location of matched keypoints
    pts1 = np.float32([keypoints1[m.queryIdx].pt for m in matches])
    pts2 = np.float32([keypoints2[m.trainIdx].pt for m in matches])

    # Find homography using RANSAC
    homography_matrix, mask = cv2.findHomography(pts1, pts2, cv2.RANSAC, 5.0)
    
    return matches, homography_matrix, mask

# Function to draw matched keypoints between two images
def draw_matches(image1, image2, keypoints1, keypoints2, matches, mask):
    # Draw only inliers (if mask is applied)
    inlier_matches = [matches[i] for i in range(len(matches)) if mask[i]]
    
    # Draw matches
    result = cv2.drawMatches(image1, keypoints1, image2, keypoints2, inlier_matches, None, 
                             flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)
    
    return result

# Read images
image1 = cv2.imread('points/images/cube2.jpg', cv2.IMREAD_GRAYSCALE)
image2 = cv2.imread('points/images/cube3.jpg', cv2.IMREAD_GRAYSCALE)

# Detect keypoints and compute descriptors for both images
keypoints1, descriptors1 = detect_and_compute(image1)
keypoints2, descriptors2 = detect_and_compute(image2)

# Match keypoints between the two images and compute the homography matrix using RANSAC
matches, homography_matrix, mask = match_keypoints_and_find_homography(image1, image2, keypoints1, keypoints2, descriptors1, descriptors2)

# Draw matches and show the result
matched_image = draw_matches(image1, image2, keypoints1, keypoints2, matches, mask)

# Display the matched keypoints
plt.figure(figsize=(12, 6))
plt.imshow(matched_image)
plt.title('Matched Keypoints with RANSAC')
plt.axis('off')
plt.show()

# Optionally, print the homography matrix
print("Homography Matrix:")
print(homography_matrix)
