import cv2
import numpy

def xml_dataloader(file_name, mat_name):
    fs = cv2.FileStorage(file_name, cv2.FILE_STORAGE_READ)
    fn = fs.getNode(mat_name)
    print (numpy.shape(fn.mat()))
    return fn.mat()
