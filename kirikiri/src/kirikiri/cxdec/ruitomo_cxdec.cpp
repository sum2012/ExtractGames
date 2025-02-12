#include "cxdec.h"

static int xcode_building_stage0(struct cxdec_xcode_status *xcode, int stage);
static int xcode_building_stage1(struct cxdec_xcode_status *xcode, int stage);

static BYTE EncryptionControlBlock[4096] = {
        0x20, 0x45, 0x6e, 0x63, 0x72, 0x79, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x63, 0x6f, 0x6e, 0x74, 
        0x72, 0x6f, 0x6c, 0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x20, 0x2d, 0x2d, 0x20, 0x53, 0x74, 0x61, 
        0x74, 0x69, 0x63, 0x61, 0x6c, 0x6c, 0x79, 0x20, 0x6f, 0x72, 0x20, 0x64, 0x79, 0x6e, 0x61, 0x6d, 
        0x69, 0x63, 0x61, 0x6c, 0x6c, 0x79, 0x2c, 0x20, 0x64, 0x69, 0x72, 0x65, 0x63, 0x74, 0x6c, 0x79, 
        0x20, 0x6f, 0x72, 0x20, 0x69, 0x6e, 0x64, 0x69, 0x72, 0x65, 0x63, 0x74, 0x6c, 0x79, 0x2c, 0x20, 
        0x75, 0x73, 0x69, 0x6e, 0x67, 0x20, 0x74, 0x68, 0x69, 0x73, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 
        0x61, 0x6d, 0x20, 0x61, 0x6e, 0x64, 0x2f, 0x6f, 0x72, 0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x20, 
        0x66, 0x72, 0x6f, 0x6d, 0x20, 0x6f, 0x74, 0x68, 0x65, 0x72, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 
        0x61, 0x6d, 0x73, 0x20, 0x77, 0x69, 0x6c, 0x6c, 0x20, 0x62, 0x65, 0x20, 0x69, 0x6c, 0x6c, 0x65, 
        0x67, 0x61, 0x6c, 0x20, 0x62, 0x79, 0x20, 0x74, 0x68, 0x65, 0x20, 0x6c, 0x69, 0x63, 0x65, 0x6e, 
        0x73, 0x65, 0x20, 0x61, 0x67, 0x72, 0x65, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x20, 0x82, 0xb1, 
        0x82, 0xcc, 0x83, 0x76, 0x83, 0x8d, 0x83, 0x4f, 0x83, 0x89, 0x83, 0x80, 0x82, 0xe2, 0x83, 0x75, 
        0x83, 0x8d, 0x83, 0x62, 0x83, 0x4e, 0x82, 0xf0, 0x81, 0x41, 0x90, 0xc3, 0x93, 0x49, 0x82, 0xc5, 
        0x82, 0xa0, 0x82, 0xea, 0x93, 0xae, 0x93, 0x49, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 0x81, 0x41, 
        0x92, 0xbc, 0x90, 0xda, 0x93, 0x49, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 0x8a, 0xd4, 0x90, 0xda, 
        0x93, 0x49, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 0x81, 0x41, 0x91, 0xbc, 0x82, 0xcc, 0x83, 0x76, 
        0x83, 0x8d, 0x83, 0x4f, 0x83, 0x89, 0x83, 0x80, 0x82, 0xa9, 0x82, 0xe7, 0x97, 0x70, 0x82, 0xa2, 
        0x82, 0xe9, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xcd, 0x83, 0x89, 0x83, 0x43, 0x83, 0x5a, 0x83, 0x93, 
        0x83, 0x58, 0x82, 0xc9, 0x82, 0xe6, 0x82, 0xe8, 0x8b, 0xd6, 0x82, 0xb6, 0x82, 0xe7, 0x82, 0xea, 
        0x82, 0xc4, 0x82, 0xa2, 0x82, 0xdc, 0x82, 0xb7, 0x81, 0x42, 0x0a, 0x81, 0x75, 0x82, 0xa8, 0x82, 
        0xcd, 0x82, 0xe6, 0x82, 0xa4, 0x82, 0xb2, 0x82, 0xb4, 0x82, 0xa2, 0x82, 0xdc, 0x82, 0xb7, 0x81, 
        0x41, 0x98, 0x61, 0x8b, 0x76, 0x92, 0xc3, 0x82, 0xb3, 0x82, 0xdc, 0x81, 0x42, 0x83, 0x4c, 0x83, 
        0x58, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xe6, 0x82, 0xeb, 0x82, 0xb5, 0x82, 0xa2, 0x82, 0xc5, 0x82, 
        0xb7, 0x82, 0xa9, 0x81, 0x48, 0x81, 0x76, 0x0a, 0x81, 0x75, 0x82, 0xbe, 0x82, 0xdf, 0x81, 0x76, 
        0x0a, 0x0a, 0x81, 0x40, 0x82, 0xb1, 0x82, 0xea, 0x82, 0xaa, 0x8e, 0x4f, 0x93, 0xfa, 0x91, 0x4f, 
        0x81, 0x42, 0x0a, 0x81, 0x40, 0x95, 0xbd, 0x89, 0xb8, 0x96, 0xb3, 0x8e, 0x96, 0x82, 0xc8, 0x92, 
        0xa9, 0x81, 0x41, 0x93, 0xaf, 0x8b, 0x89, 0x90, 0xb6, 0x82, 0xc6, 0x82, 0xcc, 0x82, 0xbd, 0x82, 
        0xed, 0x82, 0xa2, 0x82, 0xe0, 0x82, 0xc8, 0x82, 0xa2, 0x93, 0xfa, 0x8f, 0xed, 0x81, 0x42, 0x0a, 
        0x0a, 0x81, 0x75, 0x82, 0xa2, 0x82, 0xad, 0x82, 0xed, 0x82, 0xe6, 0x81, 0x41, 0x82, 0xa2, 0x82, 
        0xa2, 0x82, 0xed, 0x82, 0xcb, 0x81, 0x41, 0x8b, 0x43, 0x8d, 0x87, 0x82, 0xa2, 0x82, 0xf0, 0x93, 
        0xfc, 0x82, 0xea, 0x82, 0xc4, 0x81, 0x49, 0x81, 0x76, 0x0a, 0x81, 0x75, 0x95, 0xa8, 0x8e, 0x96, 
        0x82, 0xcd, 0x90, 0xb8, 0x90, 0x5f, 0x98, 0x5f, 0x82, 0xe6, 0x82, 0xe8, 0x8c, 0xbb, 0x8e, 0xc0, 
        0x8e, 0xe5, 0x8b, 0x60, 0x82, 0xc5, 0x81, 0x49, 0x81, 0x76, 0x0a, 0x0a, 0x81, 0x40, 0x82, 0xbb, 
        0x82, 0xcc, 0x8e, 0x4f, 0x93, 0xfa, 0x8c, 0xe3, 0x81, 0x42, 0x0a, 0x81, 0x40, 0x94, 0x52, 0x82, 
        0xa6, 0x82, 0xe9, 0x83, 0x72, 0x83, 0x8b, 0x82, 0xcc, 0x89, 0xae, 0x8f, 0xe3, 0x82, 0xa9, 0x82, 
        0xe7, 0x83, 0x5f, 0x83, 0x43, 0x83, 0x75, 0x82, 0xb5, 0x82, 0xc4, 0x81, 0x41, 0x93, 0x73, 0x8e, 
        0x73, 0x93, 0x60, 0x90, 0xe0, 0x82, 0xcc, 0x8d, 0x95, 0x82, 0xa2, 0x83, 0x89, 0x83, 0x43, 0x83, 
        0x5f, 0x81, 0x5b, 0x82, 0xc9, 0x92, 0xc7, 0x82, 0xa2, 0x82, 0xa9, 0x82, 0xaf, 0x82, 0xe7, 0x82, 
        0xea, 0x82, 0xbd, 0x81, 0x42, 0x0a, 0x0a, 0x81, 0x40, 0x89, 0x5e, 0x96, 0xbd, 0x82, 0xcd, 0x82, 
        0xa2, 0x82, 0xc2, 0x82, 0xbe, 0x82, 0xc1, 0x82, 0xc4, 0x96, 0xe2, 0x93, 0x9a, 0x96, 0xb3, 0x97, 
        0x70, 0x82, 0xc9, 0x82, 0xe2, 0x82, 0xc1, 0x82, 0xc4, 0x82, 0xad, 0x82, 0xe9, 0x81, 0x42, 0x0a, 
        0x81, 0x40, 0x8e, 0xa9, 0x95, 0xaa, 0x82, 0xbd, 0x82, 0xbf, 0x82, 0xf0, 0x82, 0xa8, 0x8d, 0x5c, 
        0x82, 0xa2, 0x82, 0xc8, 0x82, 0xb5, 0x82, 0xc9, 0x8e, 0xa9, 0x95, 0xaa, 0x8f, 0x9f, 0x8e, 0xe8, 
        0x82, 0xc9, 0x8f, 0x84, 0x82, 0xc1, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xad, 0x81, 0x42, 0x0a, 0x0a, 
        0x81, 0x40, 0x8a, 0x46, 0x8c, 0xb3, 0x82, 0xe9, 0x82, 0xa2, 0x82, 0xcd, 0x81, 0x41, 0x89, 0xc6, 
        0x82, 0xc8, 0x82, 0xb5, 0x8e, 0x71, 0x82, 0xbe, 0x82, 0xc1, 0x82, 0xbd, 0x81, 0x42, 0x0a, 0x81, 
        0x40, 0x89, 0xd4, 0x8f, 0xe9, 0x89, 0xd4, 0x8c, 0x7b, 0x82, 0xcd, 0x81, 0x41, 0x92, 0x44, 0x82, 
        0xed, 0x82, 0xea, 0x82, 0xbd, 0x82, 0xe0, 0x82, 0xcc, 0x82, 0xf0, 0x8e, 0xe6, 0x82, 0xe8, 0x95, 
        0xd4, 0x82, 0xb7, 0x82, 0xbd, 0x82, 0xdf, 0x82, 0xc9, 0x82, 0xe2, 0x82, 0xc1, 0x82, 0xc4, 0x82, 
        0xab, 0x82, 0xbd, 0x81, 0x42, 0x0a, 0x81, 0x40, 0x96, 0xc2, 0x91, 0xea, 0x82, 0xb1, 0x82, 0xe6, 
        0x82, 0xe8, 0x82, 0xcd, 0x81, 0x41, 0x8f, 0xc1, 0x82, 0xa6, 0x82, 0xbd, 0x8d, 0xa5, 0x96, 0xf1, 
        0x8e, 0xd2, 0x82, 0xf0, 0x92, 0x54, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xbd, 0x81, 0x42, 
        0x0a, 0x81, 0x40, 0x8a, 0x9d, 0x8f, 0xea, 0x88, 0xa9, 0x8e, 0x71, 0x82, 0xcd, 0x81, 0x41, 0x95, 
        0x83, 0x82, 0xcc, 0x95, 0x73, 0x8e, 0x6e, 0x96, 0x96, 0x82, 0xcc, 0x82, 0xc6, 0x82, 0xce, 0x82, 
        0xc1, 0x82, 0xbf, 0x82, 0xe8, 0x82, 0xf0, 0x8e, 0xf3, 0x82, 0xaf, 0x82, 0xc4, 0x82, 0xa2, 0x82, 
        0xbd, 0x81, 0x42, 0x0a, 0x81, 0x40, 0x94, 0x92, 0x8f, 0xe2, 0x88, 0xc9, 0x91, 0xe3, 0x82, 0xcd, 
        0x81, 0x41, 0x82, 0xd0, 0x82, 0xc6, 0x82, 0xe8, 0x82, 0xda, 0x82, 0xc1, 0x82, 0xbf, 0x82, 0xbe, 
        0x82, 0xc1, 0x82, 0xbd, 0x81, 0x42, 0x0a, 0x0a, 0x81, 0x40, 0x82, 0xbb, 0x82, 0xb5, 0x82, 0xc4, 
        0x81, 0x42, 0x0a, 0x81, 0x40, 0x94, 0x4c, 0x82, 0xa9, 0x82, 0xd4, 0x82, 0xe8, 0x82, 0xcc, 0x97, 
        0x44, 0x93, 0x99, 0x90, 0xb6, 0x81, 0x41, 0x98, 0x61, 0x8b, 0x76, 0x92, 0xc3, 0x92, 0x71, 0x82, 
        0xcd, 0x92, 0x66, 0x96, 0x96, 0x96, 0x82, 0x82, 0xbe, 0x82, 0xc1, 0x82, 0xbd, 0x81, 0x42, 0x0a, 
        0x0a, 0x81, 0x40, 0x92, 0x71, 0x82, 0xc9, 0x82, 0xcd, 0xe1, 0x7b, 0x82, 0xaa, 0x82, 0xa0, 0x82, 
        0xe9, 0x81, 0x42, 0x0a, 0x81, 0x40, 0x8f, 0x68, 0x96, 0xbd, 0x82, 0xcc, 0x82, 0xe6, 0x82, 0xa4, 
        0x82, 0xc8, 0x89, 0x5e, 0x96, 0xbd, 0x82, 0xcc, 0x82, 0xe6, 0x82, 0xa4, 0x82, 0xc8, 0x81, 0x41, 
        0xe0, 0x80, 0x88, 0xf3, 0x82, 0xdf, 0x82, 0xa2, 0x82, 0xbd, 0x8f, 0xac, 0x82, 0xb3, 0x82, 0xc8, 
        0xe1, 0x7b, 0x82, 0xbe, 0x81, 0x42, 0x0a, 0x81, 0x40, 0x82, 0xbb, 0x82, 0xcc, 0xe1, 0x7b, 0x82, 
        0xcd, 0x81, 0x41, 0x82, 0xab, 0x82, 0xc1, 0x82, 0xc6, 0x90, 0xcc, 0x82, 0xa9, 0x82, 0xe7, 0x81, 
        0x41, 0x82, 0xeb, 0x82, 0xad, 0x82, 0xc5, 0x82, 0xe0, 0x82, 0xc8, 0x82, 0xa2, 0x90, 0xe6, 0x8d, 
        0x73, 0x82, 0xab, 0x82, 0xf0, 0x97, 0x5c, 0x8d, 0x90, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xa2, 0x82, 
        0xbd, 0x82, 0xcc, 0x82, 0xbe, 0x82, 0xeb, 0x82, 0xa4, 0x81, 0x42, 0x0a, 0x0a, 0x81, 0x40, 0x8e, 
        0x80, 0x82, 0xf1, 0x82, 0xbe, 0x95, 0xea, 0x82, 0xa9, 0x82, 0xe7, 0x8e, 0xe8, 0x8e, 0x86, 0x82, 
        0xaa, 0x93, 0xcd, 0x82, 0xa2, 0x82, 0xc4, 0x88, 0xc8, 0x97, 0x88, 0x81, 0x41, 0x0a, 0x81, 0x40, 
        0x92, 0x6e, 0x97, 0x8b, 0x8c, 0xb4, 0x82, 0xc9, 0x96, 0xc0, 0x82, 0xa2, 0x8d, 0x9e, 0x82, 0xf1, 
        0x82, 0xbe, 0x82, 0xe6, 0x82, 0xa4, 0x82, 0xc9, 0x88, 0xf8, 0x82, 0xab, 0x82, 0xe0, 0x90, 0xd8, 
        0x82, 0xe7, 0x82, 0xb8, 0x83, 0x67, 0x83, 0x89, 0x83, 0x75, 0x83, 0x8b, 0x82, 0xaa, 0x89, 0x9f, 
        0x82, 0xb5, 0x82, 0xa9, 0x82, 0xaf, 0x82, 0xe9, 0x81, 0x42, 0x0a, 0x81, 0x40, 0x8f, 0x68, 0x96, 
        0xbd, 0x82, 0xcc, 0x82, 0xe6, 0x82, 0xa4, 0x82, 0xc9, 0x89, 0x5e, 0x96, 0xbd, 0x82, 0xcc, 0x82, 
        0xe6, 0x82, 0xa4, 0x82, 0xc9, 0x81, 0x41, 0x0a, 0x81, 0x40, 0x97, 0xdc, 0x96, 0xda, 0x82, 0xcc, 
        0x92, 0x71, 0x82, 0xaa, 0x8f, 0x6f, 0x89, 0xef, 0x82, 0xc1, 0x82, 0xbd, 0x8f, 0xad, 0x8f, 0x97, 
        0x82, 0xbd, 0x82, 0xbf, 0x82, 0xcc, 0x90, 0x67, 0x91, 0xcc, 0x82, 0xc9, 0x82, 0xcd, 0x81, 0x41, 
        0x92, 0x71, 0x82, 0xc6, 0x93, 0xaf, 0x82, 0xb6, 0x8c, 0x60, 0x82, 0xcc, 0xe1, 0x7b, 0x82, 0xaa, 
        0x82, 0xa0, 0x82, 0xc1, 0x82, 0xbd, 0x81, 0x42, 0x0a, 0x0a, 0x81, 0x40, 0x8c, 0xbe, 0x8c, 0xea, 
        0x93, 0xb9, 0x92, 0x66, 0x82, 0xc8, 0x8e, 0xf4, 0x82, 0xed, 0x82, 0xea, 0x82, 0xbd, 0x90, 0xc2, 
        0x8f, 0x74, 0x82, 0xc6, 0x91, 0xce, 0x9b, 0xb3, 0x82, 0xb7, 0x82, 0xe9, 0x82, 0xbd, 0x82, 0xdf, 
        0x82, 0xc9, 0x81, 0x41, 0x88, 0xea, 0x90, 0x53, 0x82, 0xc5, 0x82, 0xe0, 0x82, 0xc8, 0x82, 0xad, 
        0x93, 0xaf, 0x91, 0xcc, 0x82, 0xc5, 0x82, 0xe0, 0x82, 0xc8, 0x82, 0xa2, 0x81, 0x41, 0x0a, 0x81, 
        0x40, 0x98, 0x5a, 0x90, 0x6c, 0x82, 0xcc, 0x8f, 0xad, 0x8f, 0x97, 0x82, 0xaa, 0x93, 0xaf, 0x96, 
        0xbf, 0x82, 0xf0, 0x8c, 0x8b, 0x82, 0xd4, 0x81, 0x42, 0x0a, 0x0a, 0x81, 0x75, 0x82, 0xc2, 0x82, 
        0xdc, 0x82, 0xe8, 0x81, 0x41, 0x82, 0xb1, 0x82, 0xea, 0x82, 0xcd, 0x93, 0xaf, 0x96, 0xbf, 0x82, 
        0xbe, 0x81, 0x42, 0x94, 0x6a, 0x82, 0xe7, 0x82, 0xea, 0x82, 0xc8, 0x82, 0xa2, 0x8c, 0x5f, 0x96, 
        0xf1, 0x81, 0x41, 0x0a, 0x81, 0x40, 0x97, 0xa0, 0x90, 0xd8, 0x82, 0xe7, 0x82, 0xea, 0x82, 0xc8, 
        0x82, 0xa2, 0x90, 0xbe, 0x96, 0xf1, 0x81, 0x41, 0x82, 0xa0, 0x82, 0xe9, 0x82, 0xa2, 0x82, 0xcd, 
        0x8c, 0xdd, 0x82, 0xa2, 0x82, 0xf0, 0x94, 0x9b, 0x82, 0xe9, 0x90, 0xa7, 0x96, 0xf1, 0x82, 0xc5, 
        0x82, 0xe0, 0x82, 0xa0, 0x82, 0xe9, 0x81, 0x42, 0x0a, 0x81, 0x40, 0x96, 0x6c, 0x82, 0xbd, 0x82, 
        0xbf, 0x82, 0xcd, 0x8c, 0xfb, 0x96, 0xf1, 0x91, 0xa9, 0x82, 0xf0, 0x82, 0xa9, 0x82, 0xed, 0x82, 
        0xb7, 0x81, 0x41, 0x8e, 0x77, 0x90, 0xd8, 0x82, 0xe8, 0x82, 0xf0, 0x82, 0xb7, 0x82, 0xe9, 0x81, 
        0x41, 0x83, 0x54, 0x83, 0x43, 0x83, 0x93, 0x82, 0xf0, 0x8c, 0xf0, 0x8a, 0xb7, 0x82, 0xb5, 0x81, 
        0x41, 0x0a, 0x81, 0x40, 0x8c, 0x8c, 0x94, 0xbb, 0x8f, 0xf3, 0x82, 0xc9, 0x92, 0xa5, 0x82, 0xf0, 
        0x89, 0x9f, 0x82, 0xb5, 0x82, 0xc4, 0x81, 0x41, 0x8d, 0x95, 0x82, 0xa2, 0x97, 0x72, 0x94, 0xe7, 
        0x8e, 0x86, 0x82, 0xc9, 0x8c, 0x8c, 0x82, 0xcc, 0x83, 0x43, 0x83, 0x93, 0x83, 0x4e, 0x82, 0xc5, 
        0x82, 0xb5, 0x82, 0xbd, 0x82, 0xbd, 0x82, 0xdf, 0x82, 0xe9, 0x81, 0x76, 0x0a, 0x0a, 0x81, 0x75, 
        0x88, 0xea, 0x90, 0x6c, 0x82, 0xc5, 0x90, 0xed, 0x82, 0xa6, 0x82, 0xc8, 0x82, 0xa2, 0x82, 0xa9, 
        0x82, 0xe7, 0x97, 0xcd, 0x82, 0xf0, 0x8d, 0x87, 0x82, 0xed, 0x82, 0xb9, 0x82, 0xe9, 0x81, 0x42, 
        0x0a, 0x81, 0x40, 0x88, 0xea, 0x96, 0x7b, 0x82, 0xcc, 0x96, 0xee, 0x82, 0xaa, 0x90, 0xdc, 0x82, 
        0xea, 0x82, 0xe9, 0x82, 0xc8, 0x82, 0xe7, 0x8c, 0xdc, 0x96, 0x7b, 0x98, 0x5a, 0x96, 0x7b, 0x82, 
        0xc6, 0x91, 0xa9, 0x82, 0xcb, 0x82, 0xc4, 0x82, 0xb5, 0x82, 0xdc, 0x82, 0xa6, 0x82, 0xce, 0x82, 
        0xa2, 0x82, 0xa2, 0x81, 0x42, 0x0a, 0x81, 0x40, 0x97, 0x98, 0x8a, 0x51, 0x82, 0xcc, 0x88, 0xea, 
        0x92, 0x76, 0x82, 0xbe, 0x81, 0x42, 0x97, 0x98, 0x97, 0x70, 0x82, 0xcc, 0x8a, 0xd6, 0x8c, 0x57, 
        0x82, 0xbe, 0x81, 0x42, 0x8b, 0x43, 0x82, 0xc9, 0x93, 0xfc, 0x82, 0xe7, 0x82, 0xc8, 0x82, 0xa2, 
        0x82, 0xc6, 0x82, 0xb1, 0x82, 0xeb, 0x82, 0xc9, 0x96, 0xda, 0x82, 0xf0, 0x82, 0xc2, 0x82, 0xd4, 
        0x82, 0xe8, 0x81, 0x41, 0x0a, 0x81, 0x40, 0x91, 0x8a, 0x8e, 0xe8, 0x82, 0xcc, 0x8f, 0x47, 0x82, 
        0xc5, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xe9, 0x95, 0x94, 0x95, 0xaa, 0x82, 0xcc, 0x97, 0xcd, 0x82, 
        0xf0, 0x8e, 0xd8, 0x82, 0xe8, 0x82, 0xe9, 0x81, 0x42, 0x0a, 0x81, 0x40, 0x92, 0x4e, 0x82, 0xa9, 
        0x82, 0xcc, 0x8e, 0xb8, 0x94, 0x73, 0x82, 0xf0, 0x83, 0x74, 0x83, 0x48, 0x83, 0x8d, 0x81, 0x5b, 
        0x82, 0xb5, 0x82, 0xc4, 0x81, 0x41, 0x8e, 0xa9, 0x95, 0xaa, 0x82, 0xcc, 0x8f, 0x9f, 0x82, 0xbf, 
        0x93, 0xbe, 0x82, 0xbd, 0x82, 0xe0, 0x82, 0xcc, 0x82, 0xf0, 0x95, 0xaa, 0x82, 0xaf, 0x97, 0x5e, 
        0x82, 0xa6, 0x82, 0xe9, 0x81, 0x76, 0x0a, 0x0a, 0x81, 0x75, 0x92, 0x4e, 0x82, 0xa9, 0x82, 0xcc, 
        0x82, 0xbd, 0x82, 0xdf, 0x82, 0xb6, 0x82, 0xe1, 0x82, 0xc8, 0x82, 0xad, 0x8e, 0xa9, 0x95, 0xaa, 
        0x82, 0xcc, 0x82, 0xbd, 0x82, 0xdf, 0x82, 0xc9, 0x81, 0x41, 0x8e, 0xa9, 0x90, 0x67, 0x82, 0xcc, 
        0x82, 0xbd, 0x82, 0xdf, 0x82, 0xc9, 0x81, 0x76, 0x0a, 0x0a, 0x81, 0x75, 0x96, 0x6c, 0x82, 0xbd, 
        0x82, 0xbf, 0x82, 0xcd, 0x82, 0xd0, 0x82, 0xc6, 0x82, 0xc2, 0x82, 0xcc, 0x87, 0x80, 0x8c, 0x51, 
        0x82, 0xea, 0x87, 0x81, 0x82, 0xc9, 0x82, 0xc8, 0x82, 0xe9, 0x81, 0x42, 0x8c, 0x51, 0x82, 0xea, 
        0x82, 0xcd, 0x82, 0xa8, 0x8c, 0xdd, 0x82, 0xa2, 0x82, 0xf0, 0x8e, 0xe7, 0x82, 0xe9, 0x82, 0xbd, 
        0x82, 0xdf, 0x82, 0xcc, 0x82, 0xe0, 0x82, 0xcc, 0x82, 0xc8, 0x82, 0xf1, 0x82, 0xbe, 0x81, 0x76, 
        0x0a, 0x0a, 0x81, 0x40, 0x82, 0xa2, 0x82, 0xc2, 0x82, 0xa9, 0x97, 0x88, 0x82, 0xe9, 0x95, 0xbd, 
        0x89, 0xb8, 0x96, 0xb3, 0x8e, 0x96, 0x82, 0xc8, 0x93, 0xfa, 0x81, 0x58, 0x82, 0xf0, 0x96, 0xb2, 
        0x8c, 0xa9, 0x82, 0xc4, 0x81, 0x41, 0x91, 0x53, 0x90, 0x67, 0x91, 0x53, 0x97, 0xec, 0x82, 0xc5, 
        0x8e, 0xbe, 0x91, 0x96, 0x82, 0xb7, 0x82, 0xe9, 0x82, 0xc5, 0x82, 0xb1, 0x82, 0xda, 0x82, 0xb1, 
        0x82, 0xbe, 0x82, 0xe7, 0x82, 0xaf, 0x82, 0xcc, 0x8f, 0xad, 0x8f, 0x97, 0x82, 0xbd, 0x82, 0xbf, 
        0x82, 0xcd, 0x81, 0x41, 0x0a, 0x81, 0x40, 0x82, 0xa2, 0x82, 0xc2, 0x82, 0xb5, 0x82, 0xa9, 0x8c, 
        0xc5, 0x82, 0xa2, 0xe3, 0x4a, 0x82, 0xc5, 0x8c, 0x8b, 0x82, 0xce, 0x82, 0xea, 0x82, 0xc4, 0x82, 
        0xa2, 0x82, 0xad, 0x81, 0x42, 0x0a, 0x0a, 0x81, 0x40, 0x82, 0xaf, 0x82, 0xea, 0x82, 0xc7, 0x81, 
        0x42, 0x0a, 0x81, 0x40, 0x98, 0x61, 0x8b, 0x76, 0x92, 0xc3, 0x92, 0x71, 0x82, 0xcd, 0x92, 0x87, 
        0x8a, 0xd4, 0x82, 0xc9, 0x82, 0xe0, 0x82, 0xa2, 0x82, 0xa6, 0x82, 0xc8, 0x82, 0xa2, 0x94, 0xe9, 
        0x96, 0xa7, 0x82, 0xf0, 0x89, 0x42, 0x82, 0xb5, 0x8e, 0x9d, 0x82, 0xc1, 0x82, 0xc4, 0x82, 0xa2, 
        0x82, 0xbd, 0x81, 0x42, 0x0a, 0x0a, 0x0a, 0x81, 0x40, 0x94, 0xde, 0x8f, 0x97, 0x82, 0xcd, 0x81, 
        0x75, 0x92, 0x6a, 0x82, 0xcc, 0x8e, 0x71, 0x81, 0x76, 0x82, 0xbe, 0x82, 0xc1, 0x82, 0xbd, 0x82, 
        0xcc, 0x82, 0xbe, 0x81, 0x5c, 0x81, 0x5c, 0x81, 0x42, 0x0a, 0x0a, 0x3c, 0x44, 0xf4, 0x56, 0x73, 
        0xbd, 0x7e, 0x12, 0x9f, 0xc4, 0x3f, 0x66, 0x64, 0x43, 0xda, 0x6f, 0x55, 0x52, 0x09, 0x3e, 0x79, 
        0xf9, 0x47, 0xad, 0xa0, 0x8b, 0xf8, 0x38, 0x1d, 0xe7, 0x1f, 0x5a, 0x13, 0xd3, 0xfd, 0xf7, 0x24, 
        0x74, 0x8c, 0x17, 0xf4, 0x50, 0x14, 0x75, 0xf6, 0xf4, 0xb1, 0xa0, 0xce, 0xb3, 0x79, 0x74, 0x4e, 
        0xf1, 0x4b, 0xdd, 0x90, 0x44, 0x3a, 0x0a, 0xaf, 0x47, 0x17, 0xdf, 0x42, 0x27, 0xaa, 0xff, 0xaa, 
        0x1e, 0xcc, 0x83, 0xc7, 0xd3, 0x25, 0xe3, 0x41, 0x24, 0x91, 0x79, 0x5b, 0xbb, 0xf1, 0x0e, 0x4f, 
        0x15, 0xeb, 0x79, 0x18, 0x88, 0xd8, 0x5f, 0xb9, 0x7b, 0x1b, 0xf3, 0x5b, 0x4c, 0x93, 0x2b, 0x61, 
        0xe6, 0xa4, 0xf2, 0x6a, 0x5e, 0x9d, 0x5b, 0x8f, 0x54, 0x85, 0x99, 0xe6, 0x32, 0xff, 0xcb, 0x05, 
        0xbf, 0xc3, 0x86, 0xa2, 0x7c, 0x7c, 0x36, 0xbe, 0x86, 0x5f, 0x48, 0x85, 0x4b, 0x24, 0xf9, 0x7b, 
        0xb6, 0xfb, 0x5b, 0xfe, 0x17, 0xab, 0x1e, 0xcd, 0x0a, 0x5d, 0xf6, 0x40, 0x79, 0x6a, 0xab, 0xb9, 
        0x9e, 0x4a, 0xb7, 0x6c, 0x45, 0x8e, 0xe2, 0x17, 0xae, 0xec, 0xe7, 0x39, 0xf6, 0x9a, 0xfd, 0xbc, 
        0x01, 0x61, 0xc6, 0x3e, 0xb8, 0x54, 0xc1, 0xc0, 0x5f, 0xe7, 0x19, 0x9f, 0x71, 0x39, 0x45, 0x8b, 
        0x35, 0xf4, 0x06, 0x47, 0x1f, 0xfc, 0x08, 0xe0, 0xa1, 0xcf, 0xda, 0x88, 0xa1, 0x08, 0x20, 0x3e, 
        0x7b, 0x46, 0xce, 0xe3, 0x4c, 0x70, 0xab, 0xdc, 0x04, 0x9d, 0x9e, 0xba, 0xc1, 0x58, 0x52, 0xad, 
        0xcb, 0x4c, 0x13, 0xd9, 0x2a, 0x88, 0x67, 0xf8, 0x9e, 0x56, 0x80, 0xd2, 0x34, 0xd5, 0x1a, 0x5f, 
        0x2c, 0x60, 0xe4, 0xac, 0xb7, 0x29, 0x2a, 0xea, 0x27, 0x08, 0xf9, 0x4e, 0x62, 0xda, 0x20, 0xa7, 
        0xfb, 0x84, 0x8f, 0xdb, 0x1a, 0x47, 0xf9, 0xe4, 0x6a, 0x67, 0x4c, 0x90, 0xfc, 0xee, 0xc0, 0x3e, 
        0xc1, 0x66, 0xfb, 0xef, 0x1d, 0x00, 0x11, 0x5a, 0x11, 0x35, 0x1d, 0x2c, 0x74, 0x54, 0x40, 0x14, 
        0x83, 0xce, 0x40, 0xdb, 0xa6, 0x23, 0x53, 0x7c, 0x8b, 0x7a, 0x46, 0x3b, 0x59, 0xfe, 0xa1, 0xa0, 
        0x3e, 0xb8, 0xdd, 0xd1, 0x79, 0xc8, 0x21, 0xac, 0x9a, 0x58, 0xd4, 0x78, 0x1a, 0x9d, 0x8a, 0xe3, 
        0xf7, 0x21, 0xc1, 0xfa, 0x49, 0x0b, 0x5b, 0x9b, 0x5f, 0x9c, 0xc9, 0x52, 0x2f, 0xcb, 0xe7, 0x20, 
        0xe4, 0xc7, 0xe5, 0x0f, 0x16, 0xaa, 0x03, 0x38, 0x70, 0xb7, 0x7b, 0x6b, 0xf7, 0xa3, 0x94, 0x16, 
        0xae, 0x09, 0x30, 0x5f, 0x51, 0xaf, 0x7c, 0x33, 0x64, 0x6b, 0xb4, 0xe8, 0xf7, 0xa9, 0x8d, 0x59, 
        0xce, 0x38, 0xb4, 0xb8, 0x57, 0x21, 0xc6, 0x98, 0x16, 0xcd, 0x02, 0xc1, 0x59, 0x65, 0x5d, 0x86, 
        0x03, 0xd5, 0xff, 0x32, 0xb3, 0x25, 0xf4, 0xb6, 0x6f, 0x1d, 0xb9, 0x62, 0x3f, 0x86, 0x13, 0xfe, 
        0x69, 0x43, 0x7c, 0x70, 0xb2, 0xaf, 0x6a, 0x28, 0x51, 0x25, 0xb0, 0xba, 0x0e, 0xfa, 0xc8, 0x4d, 
        0x63, 0xd7, 0x34, 0x31, 0x68, 0x28, 0x98, 0xec, 0x18, 0x7a, 0x22, 0xbc, 0x14, 0x5d, 0x7d, 0x43, 
        0x33, 0xc6, 0x49, 0x2e, 0x1e, 0x13, 0x71, 0x9d, 0xbb, 0x76, 0x4d, 0x2d, 0x65, 0xba, 0xf9, 0xc9, 
        0xd4, 0x08, 0x99, 0xed, 0x9a, 0x76, 0x7b, 0x43, 0x7c, 0x5e, 0xf8, 0x43, 0x9d, 0x43, 0xa1, 0x30, 
        0x46, 0x23, 0xa3, 0x7e, 0x2b, 0xaa, 0xc7, 0x3a, 0x92, 0x73, 0x77, 0x3f, 0xd9, 0x50, 0x5c, 0xaf, 
        0xda, 0x42, 0x3d, 0x8d, 0x06, 0x8e, 0x4a, 0x6a, 0xc7, 0xeb, 0x59, 0x5c, 0x60, 0x90, 0xbc, 0x73, 
        0xb7, 0x7f, 0xec, 0x70, 0xf9, 0xdb, 0x27, 0x9b, 0xe8, 0x18, 0x5e, 0x7e, 0xb1, 0xd0, 0x1c, 0x44, 
        0x91, 0x26, 0xc1, 0x09, 0xd3, 0x89, 0x83, 0xd5, 0x42, 0x43, 0x0b, 0x6c, 0x7d, 0x9c, 0xdd, 0xcd, 
        0xb4, 0xe2, 0x99, 0x96, 0x2e, 0xa9, 0x21, 0x1e, 0x13, 0xf1, 0x79, 0x8d, 0xfe, 0x8c, 0x7c, 0x25, 
        0xa9, 0x55, 0x6c, 0x6f, 0x85, 0xc0, 0x78, 0xcb, 0x31, 0x2e, 0xe3, 0xdd, 0xc2, 0x98, 0x19, 0x05, 
        0x7e, 0x7b, 0xb0, 0x18, 0x02, 0x27, 0x5c, 0x63, 0x6e, 0x23, 0x4e, 0x7d, 0x50, 0xdf, 0xe0, 0xff, 
        0xa3, 0x48, 0x97, 0xd1, 0xbb, 0xdf, 0x92, 0x2a, 0x72, 0x0b, 0x7e, 0xaf, 0x83, 0x85, 0x56, 0xe0, 
        0x5b, 0x7a, 0x10, 0xf3, 0x45, 0xfd, 0xbe, 0xb2, 0x15, 0x62, 0xbe, 0x78, 0x4f, 0x75, 0x45, 0x57, 
        0x57, 0xc9, 0xe2, 0x44, 0x43, 0xfa, 0xf9, 0x20, 0x74, 0x69, 0xba, 0xb2, 0x3a, 0x3b, 0x11, 0x0d, 
        0x52, 0x07, 0x16, 0xb5, 0xc7, 0x16, 0xc6, 0xf5, 0x7a, 0x7a, 0x27, 0x8a, 0x35, 0xcd, 0xe6, 0x74, 
        0x60, 0x79, 0x7e, 0x41, 0xef, 0x66, 0x6d, 0xf2, 0xfa, 0x6a, 0xbb, 0xd0, 0xb2, 0x64, 0x88, 0x4f, 
        0x41, 0xc7, 0x79, 0xf2, 0x6c, 0xe3, 0xc3, 0xd9, 0xb4, 0xc8, 0x40, 0x45, 0x8f, 0x3f, 0x15, 0x5f, 
        0x2d, 0xc9, 0x72, 0x58, 0xb8, 0x7c, 0x00, 0x39, 0x6d, 0x5a, 0xb6, 0xc0, 0x4c, 0x7f, 0x11, 0x59, 
        0xa7, 0x8a, 0x80, 0xc1, 0x30, 0xa6, 0x8a, 0xd1, 0xad, 0x3f, 0x52, 0x3c, 0x92, 0x71, 0x78, 0xc8, 
        0xcf, 0x83, 0x46, 0x57, 0x69, 0xeb, 0x19, 0x50, 0x0a, 0x83, 0x88, 0x54, 0x7d, 0xdf, 0xd2, 0xa3, 
        0x5c, 0x01, 0x05, 0x1e, 0xde, 0x78, 0xf5, 0xa3, 0x92, 0x66, 0x38, 0x18, 0x73, 0xe0, 0xb9, 0x83, 
        0xb0, 0x71, 0x5a, 0x66, 0x92, 0x33, 0xb4, 0xd7, 0x80, 0x76, 0x21, 0x2b, 0xbd, 0xab, 0x77, 0xcc, 
        0xf8, 0xa1, 0x3d, 0x69, 0x1a, 0x45, 0xae, 0x28, 0x95, 0x86, 0x27, 0x56, 0x1d, 0x10, 0x86, 0x26, 
        0x2a, 0x21, 0x21, 0x6e, 0xc6, 0xac, 0x0d, 0x12, 0xc2, 0xa6, 0x32, 0xaf, 0x48, 0x0e, 0x7e, 0x14, 
        0x66, 0x63, 0x58, 0x05, 0x46, 0xcc, 0xc8, 0x8f, 0xfc, 0xee, 0x51, 0x06, 0x8d, 0xc2, 0xdf, 0x3d, 
        0xd9, 0x38, 0xcf, 0x6c, 0xab, 0xfe, 0xf8, 0x69, 0xa9, 0x2d, 0xb4, 0x08, 0xae, 0xf6, 0x4e, 0x01, 
        0xe9, 0xe4, 0x5f, 0x5a, 0x1f, 0x21, 0xa7, 0xca, 0xf1, 0x40, 0xc0, 0x8b, 0x27, 0xd1, 0xab, 0x65, 
        0xc4, 0xed, 0x2f, 0x42, 0xcd, 0x28, 0x9c, 0x88, 0xa0, 0xa7, 0x30, 0x05, 0xa5, 0xe6, 0xcd, 0x5a, 
        0x05, 0x57, 0x70, 0x63, 0x30, 0xac, 0x33, 0x0e, 0xe3, 0xf1, 0x92, 0x61, 0x61, 0xa8, 0x23, 0x24, 
        0x7f, 0x0b, 0xe0, 0xd6, 0xcd, 0x7b, 0x24, 0x91, 0x99, 0xf4, 0x08, 0x4a, 0x65, 0xf5, 0x25, 0x3f, 
        0x12, 0xa0, 0xea, 0xef, 0x9b, 0xd9, 0xda, 0xb6, 0x82, 0x4e, 0xf7, 0xd2, 0x34, 0x4a, 0x85, 0xfe, 
        0x76, 0xf0, 0x03, 0x90, 0xce, 0xce, 0x8d, 0xe5, 0x14, 0x39, 0x9d, 0xfc, 0x28, 0xce, 0xa9, 0x5e, 
        0x3f, 0xb7, 0x3f, 0x63, 0x96, 0xd7, 0xdb, 0x71, 0x65, 0x24, 0xf5, 0xf0, 0xb0, 0xc9, 0x93, 0x81, 
        0x69, 0x92, 0x0f, 0x70, 0x05, 0xb9, 0x6c, 0x0d, 0x7c, 0x1c, 0x75, 0x56, 0xd1, 0xae, 0xeb, 0x3e, 
        0x7c, 0x3d, 0xbb, 0xa6, 0x5a, 0x3a, 0xdb, 0xee, 0x35, 0x54, 0x95, 0x8e, 0xf8, 0xd0, 0x62, 0x49, 
        0xd9, 0x76, 0xdc, 0x44, 0x41, 0x6b, 0x95, 0x36, 0xfd, 0x6b, 0xbf, 0x97, 0xc5, 0x6b, 0x02, 0xf8, 
        0x8d, 0xd0, 0xa8, 0x23, 0x34, 0xec, 0xd4, 0x76, 0x22, 0x66, 0x33, 0xa5, 0xff, 0xcb, 0xeb, 0x9a, 
        0xce, 0xe9, 0x1c, 0x75, 0x4f, 0x25, 0xf5, 0xd0, 0x36, 0x56, 0xd8, 0x81, 0xe9, 0x19, 0x6a, 0xf8, 
        0x8d, 0xd6, 0x26, 0xa9, 0x87, 0x94, 0xc1, 0xd2, 0x9c, 0xe4, 0x42, 0x83, 0x14, 0xfb, 0x2f, 0x8e, 
        0xe2, 0x38, 0xfe, 0xba, 0x92, 0xbe, 0x49, 0x0d, 0x9b, 0xd2, 0x62, 0xba, 0xcc, 0x55, 0x73, 0xf0, 
        0xa5, 0x48, 0x6a, 0x1d, 0x36, 0xf7, 0x25, 0x1f, 0xe9, 0x1c, 0xf7, 0xf6, 0x42, 0x05, 0x6a, 0xbb, 
        0x95, 0x0a, 0x89, 0x62, 0x84, 0x9e, 0x23, 0x46, 0x43, 0xd6, 0xa6, 0xcd, 0x4e, 0x8b, 0xf1, 0xd4, 
        0x9c, 0x84, 0xbb, 0x25, 0xbc, 0x16, 0xd1, 0xa1, 0x73, 0x70, 0x99, 0x65, 0x6e, 0xd1, 0x87, 0xd4, 
        0x2a, 0x51, 0x58, 0xbf, 0x9e, 0xc4, 0xad, 0x2a, 0x1c, 0xb0, 0x7e, 0x49, 0x94, 0x00, 0xbb, 0xbd, 
        0x30, 0x3c, 0x87, 0x3f, 0x26, 0x47, 0x32, 0xdd, 0xf9, 0x56, 0x48, 0xb7, 0xcc, 0x4c, 0xae, 0x0b, 
        0xa6, 0x5e, 0xc9, 0x25, 0xf0, 0x72, 0x4f, 0x95, 0xd4, 0xfd, 0x8b, 0xc0, 0xc6, 0x4b, 0xfc, 0x9b, 
        0x40, 0x14, 0xfd, 0x29, 0x06, 0x4a, 0xf1, 0x1c, 0x7c, 0x59, 0x22, 0x59, 0xd8, 0x38, 0x0b, 0xe5, 
        0xf6, 0x4b, 0x7d, 0xce, 0x9f, 0x6e, 0x87, 0x3c, 0xef, 0xb6, 0xfd, 0x59, 0x9f, 0xd4, 0x45, 0x8b, 
        0xb4, 0x85, 0x81, 0xb5, 0x80, 0x9d, 0x50, 0x4d, 0x83, 0x18, 0x1f, 0x1b, 0xde, 0xa8, 0x31, 0x10, 
        0x49, 0x10, 0x2b, 0x88, 0xd0, 0x9f, 0x2a, 0xf9, 0x95, 0xd0, 0xdf, 0x8e, 0xc1, 0x5d, 0xfa, 0x04, 
        0xd0, 0x6d, 0x4c, 0x1d, 0x1e, 0x43, 0x65, 0xb4, 0x56, 0x43, 0xc8, 0xb2, 0x85, 0xb7, 0x86, 0x7a, 
        0x90, 0x21, 0xde, 0xe4, 0xbf, 0x56, 0x5d, 0xe6, 0x44, 0xd6, 0x39, 0xee, 0xe0, 0x77, 0x81, 0x9f, 
        0x28, 0xbe, 0xf8, 0x9b, 0x7f, 0x14, 0x95, 0x93, 0x14, 0xd4, 0xb7, 0x57, 0x2f, 0x3e, 0xf3, 0x55, 
        0x23, 0x3b, 0x22, 0xc8, 0x8c, 0xa4, 0x5c, 0x63, 0x6a, 0x7d, 0xeb, 0x0b, 0x34, 0x09, 0x47, 0x08, 
        0x95, 0xad, 0x78, 0xef, 0x42, 0xbc, 0x30, 0xc8, 0xfe, 0x41, 0x61, 0xc6, 0x98, 0x01, 0x35, 0xa9, 
        0x6d, 0xfb, 0xa9, 0x5e, 0xeb, 0xd0, 0x24, 0x75, 0x3b, 0x75, 0x6a, 0x7c, 0x09, 0x7f, 0x8a, 0x41, 
        0x75, 0xf1, 0xbf, 0xbc, 0xce, 0x3b, 0x07, 0x2e, 0xac, 0xdb, 0x39, 0xd0, 0xcf, 0x96, 0x1f, 0x91, 
        0x5e, 0x27, 0xcc, 0xe2, 0x92, 0x60, 0xc9, 0x43, 0x4f, 0x39, 0x3a, 0x3b, 0xc2, 0x06, 0x21, 0x10, 
        0xb6, 0x9b, 0x14, 0xbc, 0x8c, 0xe4, 0x5d, 0xf8, 0xf6, 0x43, 0x89, 0xc9, 0xf5, 0x51, 0xc6, 0xfc, 
        0x26, 0x05, 0x14, 0x76, 0x7e, 0x45, 0xb1, 0x3b, 0x52, 0xd3, 0xb9, 0x27, 0xc0, 0xb1, 0x20, 0xfc, 
        0x2f, 0x89, 0x55, 0xe7, 0x79, 0x60, 0xd9, 0x7d, 0x8a, 0xf0, 0xae, 0x8a, 0xc2, 0xf2, 0x54, 0x1d, 
        0x4e, 0xa1, 0xce, 0x41, 0x7c, 0x0f, 0x48, 0x3d, 0x75, 0xe5, 0xc4, 0x23, 0xfd, 0xe9, 0x34, 0xe3, 
        0x7a, 0xf2, 0xa2, 0x0f, 0xf6, 0xdb, 0x8c, 0xd6, 0xeb, 0x2c, 0x49, 0x4e, 0x70, 0x0a, 0x47, 0xea, 
        0x00, 0xcb, 0xed, 0xeb, 0x02, 0x24, 0xca, 0xb9, 0xf9, 0x23, 0x8f, 0x15, 0x87, 0x87, 0xad, 0xb5, 
        0x70, 0xde, 0xad, 0x47, 0xfa, 0x43, 0x74, 0x61, 0x5e, 0x15, 0x8f, 0xb7, 0xcc, 0x9d, 0x20, 0x7f, 
        0x3d, 0x53, 0xf7, 0x81, 0x86, 0xf0, 0x0c, 0x8b, 0xb9, 0x05, 0x62, 0xf7, 0x6c, 0x39, 0xef, 0x39, 
        0x56, 0x2d, 0x01, 0x35, 0x7d, 0x1d, 0x95, 0xd1, 0x7d, 0x4f, 0x59, 0x6a, 0x3a, 0x03, 0x34, 0x1b, 
        0xc5, 0x33, 0xb7, 0xa7, 0x83, 0x1e, 0xa3, 0x0e, 0x8f, 0x07, 0x47, 0x49, 0xd0, 0x13, 0x20, 0x47, 
        0x7e, 0xbd, 0xa8, 0x67, 0x8d, 0x80, 0xd9, 0xeb, 0x06, 0x8d, 0x14, 0x0e, 0x20, 0xfb, 0x3d, 0x42, 
        0x61, 0xc5, 0x09, 0x71, 0x9a, 0x72, 0xbb, 0xa2, 0xec, 0x49, 0x5d, 0x0b, 0xee, 0xec, 0x2e, 0x0c, 
        0x44, 0x36, 0x03, 0x6b, 0x4c, 0x1a, 0xfc, 0xca, 0x8b, 0x2c, 0xf3, 0xe1, 0x78, 0x84, 0x4a, 0x39, 
        0xea, 0x2a, 0xa3, 0x08, 0x6e, 0x40, 0x52, 0xbb, 0xd4, 0x01, 0x1a, 0xa6, 0x01, 0x9e, 0x47, 0x79, 
        0xc2, 0xd0, 0xd5, 0xd5, 0x5e, 0x27, 0x0f, 0xcb, 0x13, 0x0a, 0xe9, 0x73, 0x20, 0xa3, 0xec, 0x1d, 
        0x21, 0x83, 0x37, 0xfa, 0xa5, 0x73, 0xba, 0xf0, 0x1d, 0x65, 0x71, 0xe3, 0xd8, 0xa7, 0x7e, 0x43, 
        0x6a, 0xd7, 0xb9, 0x4c, 0x7d, 0x7e, 0x37, 0x11, 0x66, 0xc9, 0x31, 0xe8, 0x8c, 0x30, 0x38, 0x4d, 
        0x88, 0x4a, 0x51, 0x28, 0x8b, 0xb8, 0xdc, 0x2f, 0xf5, 0x04, 0x2f, 0x92, 0xa9, 0xec, 0xcd, 0xf3, 
        0x50, 0xaf, 0x49, 0xcc, 0x77, 0xb6, 0xdc, 0xe0, 0x33, 0x9e, 0x96, 0x38, 0xb2, 0x5a, 0x17, 0xea, 
        0xe5, 0x47, 0x8f, 0xba, 0x71, 0x11, 0x60, 0x17, 0x4b, 0xbb, 0x03, 0x04, 0xbd, 0x92, 0x6d, 0xa6, 
        0xdf, 0xcb, 0x27, 0xe4, 0x1d, 0x39, 0x7b, 0xa4, 0xbb, 0x55, 0x8a, 0x71, 0x4c, 0x20, 0x0a, 0x3b, 
        0xe4, 0x1c, 0xa3, 0x16, 0xa8, 0xe4, 0x86, 0x9c, 0xe3, 0x41, 0xf1, 0x21, 0xe0, 0x76, 0x61, 0xce, 
        0x42, 0xb5, 0x74, 0xf8, 0x53, 0x5a, 0xe4, 0x0c, 0x95, 0xfc, 0x13, 0x24, 0x18, 0x57, 0x4f, 0xad, 
        0xc3, 0xec, 0x94, 0xcc, 0x29, 0xda, 0x60, 0xd5, 0x56, 0xcd, 0xff, 0xa2, 0x22, 0xb4, 0xe9, 0x6e, 
        0x2c, 0x0a, 0x79, 0x00, 0x65, 0x2a, 0x6d, 0x07, 0xab, 0xa4, 0x7a, 0x64, 0x68, 0xa8, 0x3d, 0xef, 
        0x47, 0xec, 0x6b, 0xbd, 0x3f, 0xa2, 0x55, 0x70, 0xcd, 0x2b, 0xba, 0x56, 0x17, 0xd0, 0xdf, 0x62, 
        0xde, 0x7d, 0xa5, 0x77, 0xaf, 0xc2, 0x6f, 0xde, 0x29, 0x84, 0x4c, 0x5a, 0x73, 0x90, 0xfd, 0xf2, 
        0x75, 0x7a, 0x06, 0x4e, 0x7a, 0x3d, 0x60, 0x9d, 0x8c, 0x4c, 0xaf, 0x91, 0x68, 0x70, 0xbd, 0x93, 
        0xb7, 0x82, 0x96, 0xdd, 0x92, 0x8d, 0x93, 0x68, 0x44, 0xb7, 0xa8, 0x02, 0x84, 0x11, 0x0e, 0x00, 
        0xa8, 0x74, 0xd9, 0x79, 0x3b, 0x81, 0x3f, 0x8d, 0xd3, 0xa4, 0x11, 0x28, 0x20, 0x41, 0x95, 0x77, 
        0x0d, 0x91, 0xb0, 0x45, 0x35, 0xcc, 0xed, 0xd6, 0x3f, 0x1b, 0x56, 0x6e, 0xe9, 0x6d, 0x01, 0xf1, 
        0x10, 0x7d, 0x99, 0x3b, 0x3d, 0x18, 0x46, 0x5c, 0x7d, 0x6c, 0xb9, 0xd4, 0xe2, 0xaf, 0x39, 0xfe, 
        0xd6, 0x7a, 0xfb, 0x96, 0x40, 0x12, 0xe2, 0x99, 0x45, 0x11, 0xaf, 0x80, 0x04, 0x4e, 0xac, 0xf5, 
        0x9f, 0xcd, 0x0f, 0x17, 0xf9, 0x30, 0x1a, 0xb4, 0x74, 0x6f, 0xb2, 0x37, 0x51, 0x0e, 0x90, 0x84, 
        0x15, 0x5c, 0x95, 0x4c, 0x71, 0x78, 0x58, 0xe6, 0x45, 0x52, 0x4f, 0xb4, 0x4d, 0x32, 0x78, 0xd0, 
        0x9c, 0xaf, 0x2a, 0xef, 0xbf, 0xb9, 0x67, 0xb5, 0x28, 0x8f, 0xc9, 0x72, 0xa5, 0x06, 0x58, 0xf6, 
};

// @1E017420
static int xcode_building_first_stage(struct cxdec_xcode_status *xcode)
{
      switch (xcode_rand(xcode) % 3) {
      case 0:
        // MOV ESI, EncryptionControlBlock
        // MOV EAX, DWORD PTR DS:[ESI+((xcode_rand(xcode) & 0x3ff) << 2)]
        if (!push_bytexcode(xcode, 0xbe)
                || !push_dwordxcode(xcode, (DWORD)EncryptionControlBlock)
                || !push_2bytesxcode(xcode, 0x8b, 0x86)
                || !push_dwordxcode(xcode, (xcode_rand(xcode) & 0x3ff) << 2))
            return 0;
        break;
    case 2:
        // MOV EAX, xcode_rand(xcode)
        if (!push_bytexcode(xcode, 0xb8)
                || !push_dwordxcode(xcode, xcode_rand(xcode)))
            return 0;
        break;
      case 1:
          // MOV EAX, EDI
          if (!push_2bytesxcode(xcode, 0x8b, 0xc7))
              return 0;
          break;
      }
      return 1;
}

// @1E017530
static int xcode_building_stage0(struct cxdec_xcode_status *xcode, int stage)
{
    if (stage == 1)
        return xcode_building_first_stage(xcode);

    if (xcode_rand(xcode) & 1) {
        if (!xcode_building_stage1(xcode, stage - 1))
            return 0;
    } else {
        if (!xcode_building_stage0(xcode, stage - 1))
            return 0;
    }

    switch (xcode_rand(xcode) & 7) {
    case 0:
        // NOT EAX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd0))
            return 0;
        break;
    case 4:
        // DEC EAX
        if (!push_bytexcode(xcode, 0x48))
            return 0;
        break;
    case 7:
        // XOR EAX, xcode_rand(xcode)
        if (!push_bytexcode(xcode, 0x35) 
                || !push_dwordxcode(xcode, xcode_rand(xcode)))
            return 0;
        break;
    case 5:
        if (xcode_rand(xcode) & 1) {
            // ADD EAX, xcode_rand(xcode)
            if (!push_bytexcode(xcode, 0x05))
                return 0;
        } else {
            // SUB EAX, xcode_rand(xcode)
            if (!push_bytexcode(xcode, 0x2d))
                return 0;
        }
        if (!push_dwordxcode(xcode, xcode_rand(xcode)))
            return 0;
        break;
    case 1:
        // INC EAX
        if (!push_bytexcode(xcode, 0x40))
            return 0;
        break;
    case 3:
        // NEG EAX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd8))
            return 0;
        break;
    case 2:
        // PUSH EBX
        // MOV EBX, EAX
        // AND EBX, AAAAAAAA
        // AND EAX, 55555555
        // SHR EBX, 1
        // SHL EAX, 1
        // OR EAX, EBX
        // POP EBX
        if (!push_bytexcode(xcode, 0x53)
                || !push_2bytesxcode(xcode, 0x89, 0xc3)
                || !push_6bytesxcode(xcode, 0x81, 0xe3, 0xaa, 0xaa, 0xaa, 0xaa)
                || !push_5bytesxcode(xcode, 0x25, 0x55, 0x55, 0x55, 0x55)
                || !push_2bytesxcode(xcode, 0xd1, 0xeb)
                || !push_2bytesxcode(xcode, 0xd1, 0xe0)
                || !push_2bytesxcode(xcode, 0x09, 0xd8)
                || !push_bytexcode(xcode, 0x5b))
            return 0;
        break;
    case 6:
        // MOV ESI, EncryptionControlBlock
        // AND EAX, 3FFh
        // MOV EAX, DWORD PTR DS:[ESI+EAX*4]
        if (!push_bytexcode(xcode, 0xbe)
                  || !push_dwordxcode(xcode, (DWORD)EncryptionControlBlock)
                || !push_bytexcode(xcode, 0x25)
                || !push_dwordxcode(xcode, 0x3ff)
                || !push_3bytesxcode(xcode, 0x8b, 0x04, 0x86))
            return 0;
        break;
    }
    return 1;
}

// @1E017790
static int xcode_building_stage1(struct cxdec_xcode_status *xcode, int stage)
{
    if (stage == 1)
        return xcode_building_first_stage(xcode);

    // PUSH EBX
    if (!push_bytexcode(xcode, 0x53))
        return 0;

    if (xcode_rand(xcode) & 1) {
        if (!xcode_building_stage1(xcode, stage - 1))
            return 0;
    } else {
        if (!xcode_building_stage0(xcode, stage - 1))
            return 0;
    }

    // MOV EBX, EAX
    if (!push_2bytesxcode(xcode, 0x89, 0xc3))
        return 0;

    if (xcode_rand(xcode) & 1) {
        if (!xcode_building_stage1(xcode, stage - 1))
            return 0;
    } else {
        if (!xcode_building_stage0(xcode, stage - 1))
            return 0;
    }

    switch (xcode_rand(xcode) % 6) {
    case 3:
        // ADD EAX, EBX
        if (!push_2bytesxcode(xcode, 0x01, 0xd8))
            return 0;
        break;
    case 1:
        // SUB EAX, EBX
        if (!push_2bytesxcode(xcode, 0x29, 0xd8))
            return 0;
        break;
    case 4:
        // PUSH ECX
        // MOV ECX, EBX
        // AND ECX, 0F
        // SHR EAX, CL
        // POP ECX
        if (!push_bytexcode(xcode, 0x51)
                || !push_2bytesxcode(xcode, 0x89, 0xd9)
                || !push_3bytesxcode(xcode, 0x83, 0xe1, 0x0f)
                || !push_2bytesxcode(xcode, 0xd3, 0xe8)
                || !push_bytexcode(xcode, 0x59))
            return 0;
        break;
    case 2:
        // IMUL EAX, EBX
        if (!push_3bytesxcode(xcode, 0x0f,  0xaf, 0xc3))
            return 0;
        break;
    case 5:
        // NEG EAX, ADD EAX, EBX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd8) 
                || !push_2bytesxcode(xcode, 0x01, 0xd8))
            return 0;
        break;
    case 0:
        // PUSH ECX
        // MOV ECX, EBX
        // AND ECX, 0F
        // SHL EAX, CL
        // POP ECX
        if (!push_bytexcode(xcode, 0x51) 
                || !push_2bytesxcode(xcode, 0x89, 0xd9)
                || !push_3bytesxcode(xcode, 0x83, 0xe1, 0x0f)
                || !push_2bytesxcode(xcode, 0xd3, 0xe0)
                || !push_bytexcode(xcode, 0x59))
            return 0;
        break;
    }
    // POP EBX
    return push_bytexcode(xcode, 0x5b);
}

struct cxdec_callback ruitomo_cxdec_callback = {
    "ruitomo",
    { 0x2e4, 0x0c7 },
    xcode_building_stage1
};
