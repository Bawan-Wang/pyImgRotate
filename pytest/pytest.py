import numpy as np
import ctypes 
import cv2

img = cv2.imread('Lenna.jpg', cv2.IMREAD_GRAYSCALE)

w, h = img.shape
print(type(img))
print("w:", w, "h:", h)

c_ubyte_p = ctypes.POINTER(ctypes.c_uint8)
img_ptr = img.ctypes.data_as(c_ubyte_p)
new_w = ctypes.c_int()
new_h = ctypes.c_int()

rot_tool = ctypes.cdll.LoadLibrary("./imgRotate_lib.dll")

# an utility function allows easy CTypes binding
def fillprototype(f, restype, argtypes):
    f.restype = restype
    f.argtypes = argtypes
    
#main decleration
fillprototype(rot_tool.IMGRotate, ctypes.POINTER(ctypes.c_uint8), [ctypes.POINTER(ctypes.c_uint8), ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int)])

rot_image_c = rot_tool.IMGRotate(img_ptr, w, h, 165, ctypes.pointer(new_w), ctypes.pointer(new_h))
print(type(rot_image_c))
nw = new_h.value
nh = new_h.value
d = nw * nh
print("new_w:",nw,"new_h:",nh,"len:",d)

rt = np.array(np.fromiter(rot_image_c, dtype=np.uint8, count=d))

rot_img = np.reshape(rt,(nh,nw))
nnw, nnh = rot_img.shape
print(type(rot_img))
print("nw:", nnw, "nh:", nnh)

cv2.namedWindow('Lenna', cv2.WINDOW_NORMAL)
cv2.imshow('Lenna', img)

cv2.namedWindow('Lenna_rot', cv2.WINDOW_NORMAL)
cv2.imshow('Lenna_rot', rot_img)
cv2.waitKey(1)

cv2.destroyAllWindows()