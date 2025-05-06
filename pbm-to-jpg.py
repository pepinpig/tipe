from PIL import Image

# Open the PBM file
pbm_image = Image.open('points/images/dodec0-mv-15000-3-1.pbm')

# Convert to grayscale (PBM is black/white)
jpg_image = pbm_image.convert("L")

# Save as JPG
jpg_image.save("output.jpg", "JPEG")

print("Conversion complete: input.pbm → output.jpg")
