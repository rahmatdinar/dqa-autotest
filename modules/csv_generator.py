import csv
headerList = ['menu', 'params']

def createTestFile(data_test_path):
    print(data_test_path)
    with open(data_test_path, 'w', newline='') as creating_new_csv_file: 
            writer = csv.DictWriter(creating_new_csv_file, fieldnames=headerList)
            writer.writeheader()
    creating_new_csv_file.close()
def createResFile(data_res_path):
    print(data_res_path)
    with open(data_res_path, 'w', newline='') as creating_new_csv_file: 
            writer = csv.DictWriter(creating_new_csv_file, fieldnames=headerList)
            writer.writeheader()   
    creating_new_csv_file.close()
    
