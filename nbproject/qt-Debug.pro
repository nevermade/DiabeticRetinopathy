# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Debug/MinGW-Windows
TARGET = DiabeticRetinopathy
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += debug 
PKGCONFIG +=
QT = core gui widgets
SOURCES += MainForm.cpp.cc feature_extraction.cpp main.cpp preprocessing.cpp
HEADERS += MainForm.h feature_extraction.h preprocessing.h
FORMS += MainForm.ui
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/MinGW-Windows
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += sources/include 
LIBS += sources/lib/libopencv_calib3d300.dll.a sources/lib/libopencv_calib3d_pch_dephelp.a sources/lib/libopencv_core300.dll.a sources/lib/libopencv_core_pch_dephelp.a sources/lib/libopencv_features2d300.dll.a sources/lib/libopencv_features2d_pch_dephelp.a sources/lib/libopencv_flann300.dll.a sources/lib/libopencv_flann_pch_dephelp.a sources/lib/libopencv_hal300.a sources/lib/libopencv_hal_pch_dephelp.a sources/lib/libopencv_highgui300.dll.a sources/lib/libopencv_highgui_pch_dephelp.a sources/lib/libopencv_imgcodecs300.dll.a sources/lib/libopencv_imgcodecs_pch_dephelp.a sources/lib/libopencv_imgproc300.dll.a sources/lib/libopencv_imgproc_pch_dephelp.a sources/lib/libopencv_ml300.dll.a sources/lib/libopencv_ml_pch_dephelp.a sources/lib/libopencv_objdetect300.dll.a sources/lib/libopencv_objdetect_pch_dephelp.a sources/lib/libopencv_perf_calib3d_pch_dephelp.a sources/lib/libopencv_perf_core_pch_dephelp.a sources/lib/libopencv_perf_features2d_pch_dephelp.a sources/lib/libopencv_perf_imgcodecs_pch_dephelp.a sources/lib/libopencv_perf_imgproc_pch_dephelp.a sources/lib/libopencv_perf_objdetect_pch_dephelp.a sources/lib/libopencv_perf_photo_pch_dephelp.a sources/lib/libopencv_perf_stitching_pch_dephelp.a sources/lib/libopencv_perf_superres_pch_dephelp.a sources/lib/libopencv_perf_video_pch_dephelp.a sources/lib/libopencv_perf_videoio_pch_dephelp.a sources/lib/libopencv_photo300.dll.a sources/lib/libopencv_photo_pch_dephelp.a sources/lib/libopencv_shape300.dll.a sources/lib/libopencv_shape_pch_dephelp.a sources/lib/libopencv_stitching300.dll.a sources/lib/libopencv_stitching_pch_dephelp.a sources/lib/libopencv_superres300.dll.a sources/lib/libopencv_superres_pch_dephelp.a sources/lib/libopencv_test_calib3d_pch_dephelp.a sources/lib/libopencv_test_core_pch_dephelp.a sources/lib/libopencv_test_features2d_pch_dephelp.a sources/lib/libopencv_test_flann_pch_dephelp.a sources/lib/libopencv_test_highgui_pch_dephelp.a sources/lib/libopencv_test_imgcodecs_pch_dephelp.a sources/lib/libopencv_test_imgproc_pch_dephelp.a sources/lib/libopencv_test_ml_pch_dephelp.a sources/lib/libopencv_test_objdetect_pch_dephelp.a sources/lib/libopencv_test_photo_pch_dephelp.a sources/lib/libopencv_test_shape_pch_dephelp.a sources/lib/libopencv_test_stitching_pch_dephelp.a sources/lib/libopencv_test_superres_pch_dephelp.a sources/lib/libopencv_test_video_pch_dephelp.a sources/lib/libopencv_test_videoio_pch_dephelp.a sources/lib/libopencv_ts300.a sources/lib/libopencv_ts_pch_dephelp.a sources/lib/libopencv_video300.dll.a sources/lib/libopencv_video_pch_dephelp.a sources/lib/libopencv_videoio300.dll.a sources/lib/libopencv_videoio_pch_dephelp.a sources/lib/libopencv_videostab300.dll.a sources/lib/libopencv_videostab_pch_dephelp.a  
equals(QT_MAJOR_VERSION, 4) {
QMAKE_CXXFLAGS += -std=c++11
}
equals(QT_MAJOR_VERSION, 5) {
CONFIG += c++11
}
