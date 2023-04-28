import serial.tools.list_ports
import time
import datetime
import os
from paddleocr import PaddleOCR, draw_ocr
import cv2 as cv
import csv
import serial
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.metrics.pairwise import cosine_similarity
from modules.mc_chooser import checkArduinoPorts
from modules.button_ import KEY
from modules.image_process import image_writer
from modules.csv_generator import createTestFile, createResFile

current_time = datetime.datetime.now()
unique_img = "/"+ current_time.strftime("_%Y%m%d_%H%M")+ "/"
unique_file = str(current_time.strftime("_%Y%m%d_%H%M")+".csv")
img_folder_path = "D:/dqa-autotest/data-dir/data_test/img/"
data_test_path = 'D:/dqa-autotest/data-dir/data_test/'
data_txt_ref_path = 'D:/dqa-autotest/data-dir/data_reference/data_txt_ref_'
data_res_path = 'D:/dqa-autotest/data-dir/data_result/'
fonts = 'D:/dqa-autotest/font-dir/1942.ttf'

firstLayer = True
secondLayer = False
i = 0
i_max = 5
imNum = 0
testType_ = 0
prepare_stage = True
takeShot_stage = False
previousMillis = 2000
previousMillisWaitingNoResponse = 2500
start_time = time.monotonic() * 1000
cap = cv.VideoCapture(0)
main_test = False
ocr = PaddleOCR(use_angle_cls=True, lang='en')
currentRow =2 #2 for starting row both in data_test and refs
skipHeader = True
startingRefRow = 0
composure = -7
contrast = 1.0
brightness = 50
global choice 

def createImgStore(choice):
    print("createImgStore()")
    global img_path 
    img_path = img_folder_path+choice+unique_img
    if os.path.exists(img_path): pass
    else: os.mkdir(img_path)

while len(checkArduinoPorts())<=0:
    time.sleep(0.2)
    if i<1:
        print("arduino may not plugged")
        i+=1
    checkArduinoPorts()

arduinoPort = checkArduinoPorts()
print("arduino detected on : ", arduinoPort)
cap.set(cv.CAP_PROP_EXPOSURE, composure)
serial_ = serial.Serial(arduinoPort, 115200, timeout=1)
choice = str(input("select test types [dtv, atv, usb]: ").lower())
serial_.write(choice.encode())
time.sleep(0.5)
data_test_path_ = data_test_path+choice+"_test"+unique_file
data_res_path_ = data_res_path+choice+"_res"+unique_file
img_path = img_folder_path+choice+unique_img
createTestFile(data_test_path_)
createResFile(data_res_path_)
createImgStore(choice)

saveDetectedImFeature = "1"
main_test = True
    
while main_test:
    ret, frame = cap.read(0)
    frame = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    cv.imshow('cam_original', frame)
    key = cv.waitKey(100) & 0xFF
    if key != 255 and key != KEY["enter"] and key != KEY["escape"]:
        if key == KEY["w"] or key == KEY["w"]+32: composure+=0.1
        elif key == KEY["s"] or key == KEY["s"]+32 : composure-=0.5
        elif key == KEY["a"] or key == KEY["a"]+32 : brightness-=5
        elif key == KEY["d"] or key == KEY["d"]+32 : brightness+=2
        elif key == KEY["f"] or key == KEY["f"]+32 : contrast-=0.3
        elif key == KEY["h"] or key == KEY["h"]+32 : contrast+=0.1
        else : pass
        cap.set(cv.CAP_PROP_EXPOSURE, composure)
        cv.convertScaleAbs(frame, alpha=contrast, beta=brightness)
    if prepare_stage and key == KEY["enter"]:
        with open(data_txt_ref_path+choice+".csv", 'r') as ref_file:
            data_txt_ref = []
            csv_reader = csv.reader(ref_file)
            header = next(csv_reader)
            for row in csv_reader:
                data_txt_ref.append(row)
        ref_file.close()
        prepare_stage = False
        dataTx = "camIsReady"
        print(dataTx)
        serial_.write(dataTx.encode())
        time.sleep(0.2)
    if key == KEY["escape"]:
        dataTx = "abort"
        serial_.write(dataTx.encode())
        break
    else: pass
    while serial_.in_waiting>0:
        dataRx = serial_.readline().decode().rstrip()
        print(dataRx)
        if dataRx == 'takeshot':
            filename = str(imNum)
            filepath = img_path+filename+".png"
            image_writer(frame, filepath)
            #-------------------------------------#
            images_ = cv.imread(filepath)
            imNum+=1
            result = ocr.ocr(images_, cls=True)
            txts = [line[1][0].lower() for line in result[0]]
            with open(data_test_path_, mode='a+', newline='') as test_file:
                writer = csv.writer(test_file)
                writer.writerow([txts[0], list(txts[1:])])
            test_file.close()
            #need send data to arduino (ocr_finished)
            #--------------------------------------#
            data_test = txts
            print("comparing: ", data_test, " and ", data_txt_ref[startingRefRow])
            combined = [','.join(data_test), ','.join(data_txt_ref[startingRefRow])]
            if(firstLayer):
                similarity = cosine_similarity(CountVectorizer().fit_transform(combined))[0,1]
                if similarity>0.95 : status = "ok"
                elif similarity>=0.8: status = "need manually check"
                elif similarity<0.8: status = "not ok"
            elif(secondLayer):
                pass
            startingRefRow+=1

            with open(data_res_path_, mode='a+', newline='') as test_file:
                writer = csv.writer(test_file)
                writer.writerow([txts[0], similarity, status])
            test_file.close()

            #save detected images
            if saveDetectedImFeature == "1":
                boxes = [line[0] for line in result[0]]
                scores = [line[1][1] for line in result[0]]
                imagePost_ = draw_ocr(images_, boxes, txts, scores, font_path=fonts)
                cv.imwrite(filepath, imagePost_)
            elif saveDetectedImFeature == False: pass
            serial_.write("ocrFinished".encode())
        elif dataRx == 'finish': exit()      
        elif dataRx == 'firstLayerFinished': firstLayer = False; secondLayer = True
        else: print(dataRx); pass
    
serial_.close()
cap.release()
cv.destroyAllWindows()