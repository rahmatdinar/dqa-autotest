import serial.tools.list_ports
import time
import datetime
import os
from paddleocr import PaddleOCR, draw_ocr
from IPython.display import clear_output
from PIL import Image, ImageDraw, ImageFont
import cv2 as cv
import csv
import serial
import pandas as pd
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.metrics.pairwise import cosine_similarity

current_time = datetime.datetime.now()
unique_img = "/"+ current_time.strftime("_%Y%m%d_%H%M")+ "/"
unique_file = current_time.strftime("_%Y%m%d_%H%M")+".csv"
img_folder_path = "./data-dir/data_test/img/"
data_test_path = './data-dir/data_test/'
data_txt_ref_path = './data-dir/data_reference/data_txt_ref_'
data_res_path = './data-dir/data_result/'
fonts = './font-dir/fonts/1942.ttf'

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

def millis():
    return int(round((time.monotonic() * 1000) - start_time))

def checkArduinoPorts():
    serial.tools.list_ports.comports()
    arduino_ports = [
        p.device for p in serial.tools.list_ports.comports()
    ]
    arduino_ports = [x for x in arduino_ports if x not in ['COM8', 'COM9']]
    return arduino_ports

def createImgStore(choice):
    global img_path 
    img_path = img_folder_path+choice+unique_img
    if os.path.exists(img_path): pass
    else: os.mkdir(img_path)

def emptyTestFile(choice):
    headerList = ['menu', 'params']
    global data_test_path_
    data_test_path_ = data_test_path+choice+"_test"+unique_file
    if os.path.exists(data_test_path_):
        os.remove(data_test_path_)
        with open(data_test_path_, 'w', newline='') as creating_new_csv_file: 
            writer = csv.DictWriter(creating_new_csv_file, fieldnames=headerList)
            writer.writeheader()
        creating_new_csv_file.close()
    else:
        print(f"File {data_test_path_} does not exist")
        with open(data_test_path_, 'w', newline='') as creating_new_csv_file: 
            writer = csv.DictWriter(creating_new_csv_file, fieldnames=headerList)
            writer.writeheader()
        creating_new_csv_file.close()

def emptyResultFile(choice):
    headerList = ['menu', 'confident', 'status']
    global data_res_path_
    data_res_path_ = data_res_path+choice+"_res"+unique_file
    if os.path.exists(data_res_path_):
        os.remove(data_res_path_)
        with open(data_res_path_, 'w', newline='') as creating_new_csv_file: 
            writer = csv.DictWriter(creating_new_csv_file, fieldnames=headerList)
            writer.writeheader()   
        creating_new_csv_file.close()
    else:
        print(f"File {data_res_path_} does not exist")
        with open(data_res_path_, 'w', newline='') as creating_new_csv_file: 
            writer = csv.DictWriter(creating_new_csv_file, fieldnames=headerList)
            writer.writeheader()   
        creating_new_csv_file.close()

while len(checkArduinoPorts())<=0:
    time.sleep(0.2)
    if i<1:
        print("arduino may not plugged")
        i+=1
    checkArduinoPorts()

arduinoPort = checkArduinoPorts()
cap.set(cv.CAP_PROP_EXPOSURE, composure)
serial_ = serial.Serial(arduinoPort[0], 115200, timeout=1)
print("arduino port detected: ", arduinoPort); main_test = True
choice = str(input("select test types [dtv, atv, usb]: ").lower())
saveDetectedImFeature = str(input("save documentation (1/0): "))
serial_.write(choice.encode())
# saveDetectedImFeature = input("want to save the docs [True or False]: ")
while main_test:
    ret, frame = cap.read(0)
    cv.imshow('cam_original', frame)
    key = cv.waitKey(1) & 0xFF
    if prepare_stage and key == 13:
        emptyTestFile(choice)
        emptyResultFile(choice)
        createImgStore(choice)
        with open(data_txt_ref_path+choice+".csv", 'r') as ref_file:
            data_txt_ref = []
            csv_reader = csv.reader(ref_file)
            header = next(csv_reader)
            for row in csv_reader:
                data_txt_ref.append(row)
        ref_file.close()
        prepare_stage = False
        dataTx = "camIsReady"
        serial_.write(dataTx.encode())

    while serial_.in_waiting>0:
        dataRx = serial_.readline().decode().rstrip()
        if dataRx == 'takeshot':
            filename = str(imNum)
            filepath = img_path+filename+".png"
            cv.imwrite(filepath, frame)
            images_ = cv.imread(filepath)
            imNum+=1
            result = ocr.ocr(images_, cls=True)
            txts = [line[1][0].lower() for line in result[0]]
            with open(data_test_path_, mode='a+', newline='') as test_file:
                writer = csv.writer(test_file)
                writer.writerow([txts[0], list(txts[1:])])
            test_file.close()

            data_test = txts
            print("comparing: ", data_test, " and ", data_txt_ref[startingRefRow])
            combined = [','.join(data_test), ','.join(data_txt_ref[startingRefRow])]
            similarity = cosine_similarity(CountVectorizer().fit_transform(combined))[0,1]
            if similarity>0.9 : status = "ok"
            elif similarity>=0.7: status = "need manually check"
            elif similarity<0.7: status = "not ok"
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
        elif dataRx == 'finish': exit()      
        else: print(dataRx); pass

    if key == 113:
        dataTx = "abort"
        serial_.write(dataTx.encode())
        break
    elif key == (119 or 87): # w key
        composure+=0.1
        cap.set(cv.CAP_PROP_EXPOSURE, composure)
    elif key == (115 or 83): # s key
        composure-=0.5
        cap.set(cv.CAP_PROP_EXPOSURE, composure)
    elif key == (97 or 65): # a key
        brightness-=5
        cv.convertScaleAbs(frame, alpha=contrast, beta=brightness)
    elif key == (100 or 68): # d key
        brightness+=2
        cv.convertScaleAbs(frame, alpha=contrast, beta=brightness)
    elif key == (102 or 70): # f key
        contrast-=0.3
        cv.convertScaleAbs(frame, alpha=contrast, beta=brightness)
    elif key == (104 or 72): # h key
        contrast+=0.1
        cv.convertScaleAbs(frame, alpha=contrast, beta=brightness)
    else: pass
serial_.close()
cap.release()
cv.destroyAllWindows()