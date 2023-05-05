import cv2 as cv
from modules import button_

KEY = button_.KEY

def image_writer(image, filepath):
    return cv.imwrite(filepath, image)

def image_adjustment(cap, frame, key):
    if key == KEY["w"] or key == KEY["w"]+32: composure+=0.1
    elif key == KEY["s"] or key == KEY["s"]+32 : composure-=0.5
    elif key == KEY["a"] or key == KEY["a"]+32 : brightness-=5
    elif key == KEY["d"] or key == KEY["d"]+32 : brightness+=2
    elif key == KEY["f"] or key == KEY["f"]+32 : contrast-=0.3
    elif key == KEY["h"] or key == KEY["h"]+32 : contrast+=0.1
    else : pass
    return cap.set(cv.CAP_PROP_EXPOSURE, composure) and cv.convertScaleAbs(frame, alpha=contrast, beta=brightness)
