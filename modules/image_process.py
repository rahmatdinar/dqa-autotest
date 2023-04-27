import cv2 as cv

def image_writer(image, filepath):
    cv.imwrite(filepath, image)