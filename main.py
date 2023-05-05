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
from modules.button_ import KEY
from modules.image_process import image_writer, image_adjustment
from modules.csv_generator import createTestFile, createResFile

def createImgStore(choice):
    print("createImgStore()")
    global img_path 
    img_path = img_folder_path+choice+unique_img
    if os.path.exists(img_path): pass
    else: os.mkdir(img_path)

def checkArduinoPorts():
    serial.tools.list_ports.comports()
    arduino_ports = [
        p.device for p in serial.tools.list_ports.comports()
    ]
    arduino_ports = [x for x in arduino_ports if x not in ['COM8', 'COM9']]
    return arduino_ports

if __name__ == "__main__":
    current_time = datetime.datetime.now()
    unique_img = "/"+ current_time.strftime("_%Y%m%d_%H%M")+ "/"
    unique_file = str(current_time.strftime("_%Y%m%d_%H%M")+".csv")
    img_folder_path = "D:/dqa-autotest/data-dir/data_test/img/"
    data_test_path = 'D:/dqa-autotest/data-dir/data_test/'
    data_txt_ref_path = 'D:/dqa-autotest/data-dir/data_reference/data_txt_ref_'
    data_res_path = 'D:/dqa-autotest/data-dir/data_result/'
    fonts = 'D:/dqa-autotest/font-dir/1942.ttf'


    global choice 
    firstLayer = True
    secondLayer = False
    imNum = 0
    i = 0
    testType_ = 0
    prepare_stage = True
    takeShot_stage = False
    cap = cv.VideoCapture(0)
    main_test = False
    ocr = PaddleOCR(use_angle_cls=True, lang='en')
    currentRow =2 #2 for starting row both in data_test and refs
    skipHeader = True
    refRow = 0
    secondLayerCheck=0
    img_params ={
        "composure": 0, 
        "brightness": 0,
        "contrast": 0
    }

    while len(checkArduinoPorts())<=0:
        time.sleep(0.2)
        if i<1:
            print("arduino may not plugged")
            i+=1
        checkArduinoPorts()

    arduinoPort = checkArduinoPorts()
    print("arduino detected on : ", arduinoPort)
    cap.set(cv.CAP_PROP_EXPOSURE, img_params["composure"])
    serial_ = serial.Serial(arduinoPort[0], 115200, timeout=1)
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
    statusWrite = True


    while main_test:
        ret, frame = cap.read(0)
        frame = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
        cv.imshow('cam_original', frame)
        key = cv.waitKey(100) & 0xFF
        
        if key != 255 and key != KEY["enter"] and key != KEY["escape"]:
            image_adjustment(cap, frame, key, img_params)
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
        while serial_.in_waiting>0:
            dataRx = serial_.readline().decode().rstrip()
            print(dataRx)
            if dataRx == 'takeshot':
                filename = str(imNum)
                filepath = img_path+filename+".png"
                # cv.imwrite(filepath, frame)
                image_writer(frame, filepath)
                #-------------------------------------#
                images_ = cv.imread(filepath)
                imNum+=1
                result = ocr.ocr(images_, cls=True)
                txts = [line[1][0].lower() for line in result[0]]
                if(firstLayer):
                    statusWrite = True
                    with open(data_test_path_, mode='a+', newline='') as test_file:
                        writer = csv.writer(test_file)
                        writer.writerow([txts[0], list(txts[1:])])
                    test_file.close()
                    #need send data to arduino (ocr_finished)
                    #--------------------------------------#
                    data_test = txts
                    print("comparing: ", data_test, " and ", data_txt_ref[refRow])
                    combined = [','.join(data_test), ','.join(data_txt_ref[refRow])]
                    refRow+=1
                    similarity = cosine_similarity(CountVectorizer().fit_transform(combined))[0,1]
                    if similarity>0.95 : status = "ok"
                    elif similarity>=0.8: status = "need manually check"
                    elif similarity<0.8: status = "not ok"
                elif(secondLayer):
                    print(txts)
                    if secondLayerCheck>0:
                        data_test = txts
                        print("comparing: ", data_test, " and ", data_txt_ref[refRow])
                        combined = [','.join(data_test), ','.join(data_txt_ref[refRow])]
                        similarity = cosine_similarity(CountVectorizer().fit_transform(combined))[0,1]
                        if similarity>=0.99 : status = "ok"
                        elif similarity<0.99: status = "not ok"
                        refRow+=1
                        statusWrite = True
                        secondLayerCheck+=1
                        if secondLayerCheck>2: secondLayerCheck=0
                    else:
                        serial_.write(txts[1].encode())
                        statusWrite = False
                        secondLayerCheck+=1

                if statusWrite:
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
                serial_.write("ocrFinished".encode())
            elif dataRx == 'finish': exit()      
            elif dataRx == 'firstLayerFinished': firstLayer = False; secondLayer = True
            else: print(dataRx); pass
        
    serial_.close()
    cap.release()
    cv.destroyAllWindows()