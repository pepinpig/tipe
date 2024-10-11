# Import required libraries
import matplotlib.pyplot as plt
import numpy as np
import cv2

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

# Function to process an image with Harris Corner Detection
def process_image(image_path):
    # Read the image
    image = cv2.imread(image_path)

    # Make a copy of the image and convert color from BGR to RGB
    image_copy = cv2.cvtColor(np.copy(image), cv2.COLOR_BGR2RGB)

    # Convert image to grayscale
    gray = cv2.cvtColor(image_copy, cv2.COLOR_RGB2GRAY)
    gray = np.float32(gray)

    # Detect corners using Harris Corner Detection
    dst = cv2.cornerHarris(gray, 2, 3, 0.04)

    # Dilate corner image to enhance corner points
    dst = cv2.dilate(dst, None)

    # Define threshold to select strong corners
    thresh = 0.1 * dst.max()

    # Create an image copy to draw corners
    corner_image = np.copy(image_copy)

    # Iterate through all the corners and draw them on the image if they pass the threshold
    for j in range(0, dst.shape[0]):
        for i in range(0, dst.shape[1]):
            if dst[j, i] > thresh:
                # Draw circles on corners
                cv2.circle(corner_image, (i, j), 1, (0, 255, 0), 1)

    return image_copy, corner_image

# Process the first image (waffle1)
original1, corners1 = process_image('images/telecommande3.jpg')

# Process the second image (waffle2)
original2, corners2 = process_image('images/telecommande4.jpg')

# Display the original images and the corner-detected images side by side
show_images_side_by_side(corners1, corners2, title1='Corners Detected 1', title2='Corners Detected 2')
