import cv2
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from skimage.feature import graycomatrix, graycoprops
import os



# ---------------------------
# Color Feature Extraction (RGB Histogram)
# ---------------------------
def extract_color_histogram(image):
    chans = cv2.split(image)  # Split into R, G, B
    colors = ("b", "g", "r")
    features = []
    plt.figure(figsize=(10, 4))
    plt.title("Color Histogram")
    plt.xlabel("Bins")
    plt.ylabel("# of Pixels")
    for (chan, color) in zip(chans, colors):
        hist = cv2.calcHist([chan], [0], None, [256], [0, 256])
        features.extend(hist.flatten())
        plt.plot(hist, color=color)
        plt.xlim([0, 256])
    plt.show()
    return np.array(features)

# ---------------------------
# Texture Feature Extraction (GLCM)
# ---------------------------
def extract_texture_features(image_gray):
    glcm = graycomatrix(image_gray,
                        distances=[1],
                        angles=[0, np.pi/4, np.pi/2, 3*np.pi/4],
                        symmetric=True,
                        normed=True)
    features = {
        "contrast": graycoprops(glcm, "contrast").mean(),
        "dissimilarity": graycoprops(glcm, "dissimilarity").mean(),
        "homogeneity": graycoprops(glcm, "homogeneity").mean(),
        "energy": graycoprops(glcm, "energy").mean(),
        "correlation": graycoprops(glcm, "correlation").mean()
    }
    return features

# ---------------------------
# Main
# ---------------------------
if __name__ == "__main__":
    # Load image
    script_dir = os.path.dirname(os.path.abspath(__file__))  # folder where script is
    image_path = os.path.join(script_dir, "sample.jpeg")   
    image = cv2.imread(image_path)

    if image is None:
        print("❌ Error: Image not found. Check the path!")
        exit()

    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

    # Extract color features
    color_features = extract_color_histogram(image)
    print("Extracted Color Features (histogram length):", len(color_features))

    # Convert to grayscale for texture
    gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)

    # Extract texture features
    texture_features = extract_texture_features(gray)
    print("Extracted Texture Features:")
    for k, v in texture_features.items():
        print(f"{k}: {v:.4f}")

    # ---------------------------
    # Save features into CSV
    # ---------------------------
    all_features = {
        **{f"color_bin_{i}": val for i, val in enumerate(color_features)},
        **texture_features
    }

    df = pd.DataFrame([all_features])
    df.to_csv("features.csv", index=False)

    print("\n✅ Features saved to features.csv")
