import cv2 as cv

def image_writer(image, filepath):
    return cv.imwrite(filepath, image)