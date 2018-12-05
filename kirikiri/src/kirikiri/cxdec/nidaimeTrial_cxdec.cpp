#include "cxdec.h"

static int xcode_building_stage0(struct cxdec_xcode_status *xcode, int stage);
static int xcode_building_stage1(struct cxdec_xcode_status *xcode, int stage);

static BYTE EncryptionControlBlock[4096] = {
        0x20, 0x45, 0x6e, 0x63, 0x72, 0x79, 0x70, 0x74, 
        0x69, 0x6f, 0x6e, 0x20, 0x63, 0x6f, 0x6e, 0x74, 
        0x72, 0x6f, 0x6c, 0x20, 0x62, 0x6c, 0x6f, 0x63, 
        0x6b, 0x20, 0x2d, 0x2d, 0x20, 0x53, 0x74, 0x61, 
        0x74, 0x69, 0x63, 0x61, 0x6c, 0x6c, 0x79, 0x20, 
        0x6f, 0x72, 0x20, 0x64, 0x79, 0x6e, 0x61, 0x6d, 
        0x69, 0x63, 0x61, 0x6c, 0x6c, 0x79, 0x2c, 0x20, 
        0x64, 0x69, 0x72, 0x65, 0x63, 0x74, 0x6c, 0x79, 
        0x20, 0x6f, 0x72, 0x20, 0x69, 0x6e, 0x64, 0x69, 
        0x72, 0x65, 0x63, 0x74, 0x6c, 0x79, 0x2c, 0x20, 
        0x75, 0x73, 0x69, 0x6e, 0x67, 0x20, 0x74, 0x68, 
        0x69, 0x73, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 
        0x61, 0x6d, 0x20, 0x61, 0x6e, 0x64, 0x2f, 0x6f, 
        0x72, 0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x20, 
        0x66, 0x72, 0x6f, 0x6d, 0x20, 0x6f, 0x74, 0x68, 
        0x65, 0x72, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 
        0x61, 0x6d, 0x73, 0x20, 0x77, 0x69, 0x6c, 0x6c, 
        0x20, 0x62, 0x65, 0x20, 0x69, 0x6c, 0x6c, 0x65, 
        0x67, 0x61, 0x6c, 0x20, 0x62, 0x79, 0x20, 0x74, 
        0x68, 0x65, 0x20, 0x6c, 0x69, 0x63, 0x65, 0x6e, 
        0x73, 0x65, 0x20, 0x61, 0x67, 0x72, 0x65, 0x65, 
        0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x20, 0x82, 0xb1, 
        0x82, 0xcc, 0x83, 0x76, 0x83, 0x8d, 0x83, 0x4f, 
        0x83, 0x89, 0x83, 0x80, 0x82, 0xe2, 0x83, 0x75, 
        0x83, 0x8d, 0x83, 0x62, 0x83, 0x4e, 0x82, 0xf0, 
        0x81, 0x41, 0x90, 0xc3, 0x93, 0x49, 0x82, 0xc5, 
        0x82, 0xa0, 0x82, 0xea, 0x93, 0xae, 0x93, 0x49, 
        0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 0x81, 0x41, 
        0x92, 0xbc, 0x90, 0xda, 0x93, 0x49, 0x82, 0xc5, 
        0x82, 0xa0, 0x82, 0xea, 0x8a, 0xd4, 0x90, 0xda, 
        0x93, 0x49, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 
        0x81, 0x41, 0x91, 0xbc, 0x82, 0xcc, 0x83, 0x76, 
        0x83, 0x8d, 0x83, 0x4f, 0x83, 0x89, 0x83, 0x80, 
        0x82, 0xa9, 0x82, 0xe7, 0x97, 0x70, 0x82, 0xa2, 
        0x82, 0xe9, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xcd, 
        0x83, 0x89, 0x83, 0x43, 0x83, 0x5a, 0x83, 0x93, 
        0x83, 0x58, 0x82, 0xc9, 0x82, 0xe6, 0x82, 0xe8, 
        0x8b, 0xd6, 0x82, 0xb6, 0x82, 0xe7, 0x82, 0xea, 
        0x82, 0xc4, 0x82, 0xa2, 0x82, 0xdc, 0x82, 0xb7, 
        0x81, 0x42, 0x0a, 0x96, 0x82, 0x96, 0x40, 0x82, 
        0xcc, 0x8d, 0x91, 0x81, 0x41, 0x81, 0x75, 0x83, 
        0x6a, 0x83, 0x93, 0x83, 0x4c, 0x83, 0x87, 0x83, 
        0x69, 0x81, 0x5b, 0x83, 0x54, 0x81, 0x76, 0x81, 
        0x42, 0x0a, 0x82, 0xbb, 0x82, 0xcc, 0x95, 0xbd, 
        0x98, 0x61, 0x82, 0xc8, 0x8d, 0x91, 0x82, 0xcd, 
        0x81, 0x41, 0x8c, 0xbb, 0x8e, 0xc0, 0x90, 0xa2, 
        0x8a, 0x45, 0x82, 0xc9, 0x8f, 0x5a, 0x82, 0xde, 
        0x90, 0x6c, 0x81, 0x58, 0x82, 0xcc, 0x91, 0xbc, 
        0x90, 0x6c, 0x82, 0xf0, 0x8e, 0x76, 0x82, 0xa2, 
        0x82, 0xe2, 0x82, 0xe9, 0x90, 0x53, 0x82, 0xf0, 
        0x97, 0xc6, 0x82, 0xc6, 0x82, 0xb5, 0x82, 0xc4, 
        0x90, 0xac, 0x82, 0xe8, 0x97, 0xa7, 0x82, 0xc1, 
        0x82, 0xc4, 0x82, 0xa2, 0x82, 0xbd, 0x81, 0x42, 
        0x0a, 0x82, 0xbe, 0x82, 0xaa, 0x81, 0x41, 0x92, 
        0x4e, 0x82, 0xe0, 0x82, 0xaa, 0x8e, 0xa9, 0x95, 
        0xaa, 0x82, 0xcc, 0x82, 0xb1, 0x82, 0xc6, 0x82, 
        0xce, 0x82, 0xa9, 0x82, 0xe8, 0x82, 0xf0, 0x8d, 
        0x6c, 0x82, 0xa6, 0x81, 0x41, 0x8e, 0x76, 0x82, 
        0xa2, 0x82, 0xe2, 0x82, 0xe8, 0x82, 0xcc, 0x90, 
        0x53, 0x81, 0x81, 0x8b, 0xa0, 0x8b, 0x43, 0x28, 
        0x82, 0xa8, 0x82, 0xc6, 0x82, 0xb1, 0x82, 0xac, 
        0x29, 0x82, 0xf0, 0x8e, 0xb8, 0x82, 0xa2, 0x82, 
        0xc2, 0x82, 0xc2, 0x82, 0xa0, 0x82, 0xe9, 0x90, 
        0xa2, 0x82, 0xcc, 0x92, 0x86, 0x81, 0x41, 0x0a, 
        0x96, 0x82, 0x96, 0x40, 0x82, 0xcc, 0x8d, 0x91, 
        0x82, 0xcd, 0x96, 0xc5, 0x82, 0xd1, 0x82, 0xcc, 
        0x8a, 0xeb, 0x8b, 0x40, 0x82, 0xc9, 0x95, 0x6d, 
        0x82, 0xb5, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xbd, 
        0x81, 0x63, 0x81, 0x42, 0x0a, 0x82, 0xbb, 0x82, 
        0xcc, 0x96, 0x82, 0x96, 0x40, 0x82, 0xcc, 0x8d, 
        0x91, 0x82, 0xcc, 0x83, 0x73, 0x83, 0x93, 0x83, 
        0x60, 0x82, 0xf0, 0x8b, 0x7e, 0x82, 0xa4, 0x82, 
        0xbd, 0x82, 0xdf, 0x8c, 0xbb, 0x8e, 0xc0, 0x90, 
        0xa2, 0x8a, 0x45, 0x82, 0xc9, 0x8c, 0xad, 0x82, 
        0xed, 0x82, 0xb3, 0x82, 0xea, 0x82, 0xbd, 0x82, 
        0xcc, 0x82, 0xaa, 0x81, 0x41, 0x90, 0xe6, 0x91, 
        0xe3, 0x82, 0xcc, 0x96, 0x82, 0x96, 0x40, 0x8f, 
        0xad, 0x8f, 0x97, 0x81, 0x42, 0x0a, 0x94, 0xde, 
        0x8f, 0x97, 0x82, 0xcd, 0x90, 0xa2, 0x82, 0xcc, 
        0x92, 0x86, 0x82, 0xc9, 0x82, 0xd9, 0x82, 0xf1, 
        0x82, 0xcc, 0x8f, 0xad, 0x82, 0xb5, 0x8e, 0x63, 
        0x82, 0xb3, 0x82, 0xea, 0x82, 0xbd, 0x81, 0x75, 
        0x8b, 0xa0, 0x8b, 0x43, 0x81, 0x76, 0x82, 0xf0, 
        0x8f, 0x57, 0x82, 0xdf, 0x81, 0x41, 0x81, 0x75, 
        0x83, 0x6a, 0x83, 0x93, 0x83, 0x4c, 0x83, 0x87, 
        0x83, 0x69, 0x81, 0x5b, 0x83, 0x54, 0x81, 0x76, 
        0x82, 0xf0, 0x96, 0xc5, 0x82, 0xd1, 0x82, 0xa9, 
        0x82, 0xe7, 0x8b, 0x7e, 0x82, 0xc1, 0x82, 0xbd, 
        0x82, 0xcc, 0x82, 0xbe, 0x82, 0xc1, 0x82, 0xbd, 
        0x81, 0x42, 0x0a, 0x0a, 0x82, 0xbb, 0x82, 0xb5, 
        0x82, 0xc4, 0x81, 0x41, 0x82, 0xbb, 0x82, 0xea, 
        0x82, 0xa9, 0x82, 0xe7, 0x8f, 0x5c, 0x90, 0x94, 
        0x94, 0x4e, 0x81, 0x42, 0x0a, 0x81, 0x75, 0x83, 
        0x6a, 0x83, 0x93, 0x83, 0x4c, 0x83, 0x87, 0x83, 
        0x69, 0x81, 0x5b, 0x83, 0x54, 0x81, 0x76, 0x82, 
        0xcd, 0x8d, 0xc4, 0x82, 0xd1, 0x96, 0xc5, 0x82, 
        0xd1, 0x82, 0xcc, 0x89, 0x8f, 0x82, 0xc9, 0x82, 
        0xa0, 0x82, 0xc1, 0x82, 0xbd, 0x81, 0x42, 0x0a, 
        0x82, 0xa9, 0x82, 0xc2, 0x82, 0xc4, 0x82, 0xc6, 
        0x93, 0xaf, 0x82, 0xb6, 0x82, 0xad, 0x81, 0x41, 
        0x90, 0x6c, 0x81, 0x58, 0x82, 0xaa, 0x8b, 0xa0, 
        0x8b, 0x43, 0x82, 0xf0, 0x8e, 0xb8, 0x82, 0xc1, 
        0x82, 0xc4, 0x82, 0xb5, 0x82, 0xdc, 0x82, 0xc1, 
        0x82, 0xbd, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xbe, 
        0x82, 0xaf, 0x82, 0xc5, 0x82, 0xc8, 0x82, 0xad, 
        0x81, 0x41, 0x96, 0x82, 0x8a, 0x45, 0x83, 0x7d, 
        0x83, 0x74, 0x83, 0x42, 0x83, 0x41, 0x82, 0xc9, 
        0x82, 0xe0, 0x91, 0x5f, 0x82, 0xed, 0x82, 0xea, 
        0x82, 0xc4, 0x82, 0xa2, 0x82, 0xbd, 0x82, 0xcc, 
        0x82, 0xbe, 0x81, 0x42, 0x0a, 0x88, 0xd9, 0x90, 
        0xa2, 0x8a, 0x45, 0x82, 0xf0, 0x8e, 0x9f, 0x81, 
        0x58, 0x82, 0xc6, 0x92, 0x6e, 0x8f, 0xe3, 0x82, 
        0xb0, 0x82, 0xb5, 0x81, 0x41, 0x0a, 0x89, 0xe4, 
        0x82, 0xaa, 0x95, 0xa8, 0x82, 0xc6, 0x82, 0xb5, 
        0x82, 0xc4, 0x82, 0xa2, 0x82, 0xbd, 0x96, 0x82, 
        0x8a, 0x45, 0x83, 0x7d, 0x83, 0x74, 0x83, 0x42, 
        0x83, 0x41, 0x82, 0xbd, 0x82, 0xbf, 0x82, 0xcd, 
        0x81, 0x41, 0x81, 0x75, 0x83, 0x6a, 0x83, 0x93, 
        0x83, 0x4c, 0x83, 0x87, 0x83, 0x69, 0x81, 0x5b, 
        0x83, 0x54, 0x81, 0x76, 0x82, 0xcc, 0x8a, 0xeb, 
        0x8b, 0x40, 0x82, 0xf0, 0x95, 0x71, 0x8a, 0xb4, 
        0x82, 0xc9, 0x8e, 0x40, 0x82, 0xb5, 0x81, 0x41, 
        0x0a, 0x83, 0x6e, 0x83, 0x43, 0x83, 0x47, 0x83, 
        0x69, 0x82, 0xcc, 0x82, 0xb2, 0x82, 0xc6, 0x82, 
        0xad, 0x94, 0x97, 0x82, 0xc1, 0x82, 0xc4, 0x82, 
        0xad, 0x82, 0xe9, 0x81, 0x42, 0x0a, 0x82, 0xbe, 
        0x82, 0xaa, 0x81, 0x41, 0x90, 0xe6, 0x91, 0xe3, 
        0x96, 0x82, 0x96, 0x40, 0x8f, 0xad, 0x8f, 0x97, 
        0x82, 0xcd, 0x8a, 0xf9, 0x82, 0xc9, 0x96, 0x53, 
        0x82, 0xad, 0x81, 0x41, 0x8c, 0xbb, 0x8e, 0xc0, 
        0x90, 0xa2, 0x8a, 0x45, 0x82, 0xc5, 0x90, 0xb6, 
        0x8a, 0x88, 0x82, 0xb7, 0x82, 0xe9, 0x96, 0x82, 
        0x96, 0x40, 0x8f, 0xad, 0x8f, 0x97, 0x82, 0xcc, 
        0x97, 0x42, 0x88, 0xea, 0x82, 0xcc, 0x8e, 0x71, 
        0x8b, 0x9f, 0x82, 0xcd, 0x92, 0x6a, 0x82, 0xcc, 
        0x8e, 0x71, 0x81, 0x42, 0x0a, 0x8d, 0xa1, 0x82, 
        0xdc, 0x82, 0xb3, 0x82, 0xc9, 0x81, 0x75, 0x83, 
        0x6a, 0x83, 0x93, 0x83, 0x4c, 0x83, 0x87, 0x83, 
        0x69, 0x81, 0x5b, 0x83, 0x54, 0x81, 0x76, 0x8e, 
        0x6e, 0x82, 0xdc, 0x82, 0xc1, 0x82, 0xc4, 0x88, 
        0xcb, 0x97, 0x8a, 0x82, 0xcc, 0x8a, 0xeb, 0x8b, 
        0x40, 0x82, 0xc9, 0x81, 0x41, 0x0a, 0x81, 0x75, 
        0x8b, 0xa0, 0x8b, 0x43, 0x81, 0x76, 0x88, 0xec, 
        0x82, 0xea, 0x82, 0xe9, 0x8f, 0xad, 0x8f, 0x97, 
        0x82, 0xcc, 0x93, 0x6f, 0x8f, 0xea, 0x82, 0xaa, 
        0x81, 0x41, 0x8b, 0xad, 0x82, 0xad, 0x8b, 0xad, 
        0x82, 0xad, 0x91, 0xd2, 0x82, 0xbf, 0x96, 0x5d, 
        0x82, 0xdc, 0x82, 0xea, 0x82, 0xc4, 0x82, 0xa2, 
        0x82, 0xbd, 0x81, 0x42, 0x0a, 0x0a, 0x0a, 0x81, 
        0x63, 0x82, 0xbb, 0x82, 0xea, 0x82, 0xcd, 0x82, 
        0xbb, 0x82, 0xea, 0x82, 0xc6, 0x82, 0xb5, 0x82, 
        0xc4, 0x81, 0x41, 0x8c, 0xbb, 0x8e, 0xc0, 0x90, 
        0xa2, 0x8a, 0x45, 0x81, 0x42, 0x0a, 0x82, 0xbb, 
        0x82, 0xf1, 0x82, 0xc8, 0x96, 0x82, 0x96, 0x40, 
        0x82, 0xcc, 0x8d, 0x91, 0x82, 0xcc, 0x8a, 0xeb, 
        0x8b, 0x40, 0x82, 0xc8, 0x82, 0xc7, 0x82, 0xcd, 
        0x82, 0xc2, 0x82, 0xe4, 0x92, 0x6d, 0x82, 0xe7, 
        0x82, 0xb8, 0x81, 0x41, 0x8d, 0xa1, 0x93, 0xfa, 
        0x82, 0xe0, 0x8a, 0x79, 0x82, 0xb5, 0x82, 0xad, 
        0x82, 0xcc, 0x82, 0xd9, 0x82, 0xd9, 0x82, 0xf1, 
        0x82, 0xc6, 0x90, 0xb6, 0x82, 0xab, 0x82, 0xc4, 
        0x82, 0xa2, 0x82, 0xe9, 0x8e, 0xe5, 0x90, 0x6c, 
        0x8c, 0xf6, 0x81, 0x41, 0x90, 0x9b, 0x8c, 0xb4, 
        0x95, 0xb6, 0x91, 0xbe, 0x98, 0x59, 0x81, 0x42, 
        0x0a, 0x93, 0x56, 0x8a, 0x55, 0x8c, 0xc7, 0x93, 
        0xc6, 0x82, 0xcc, 0x90, 0x67, 0x82, 0xc5, 0x82, 
        0xcd, 0x82, 0xa0, 0x82, 0xe9, 0x82, 0xe0, 0x82, 
        0xcc, 0x82, 0xcc, 0x81, 0x41, 0x0a, 0x90, 0xb6, 
        0x97, 0x88, 0x82, 0xcc, 0x8f, 0x97, 0x8d, 0x44, 
        0x82, 0xab, 0x82, 0xf0, 0x91, 0xb6, 0x95, 0xaa, 
        0x82, 0xc9, 0x94, 0xad, 0x8a, 0xf6, 0x82, 0xb5, 
        0x82, 0xc4, 0x92, 0xb4, 0x91, 0x4f, 0x8c, 0xfc, 
        0x82, 0xab, 0x82, 0xc9, 0x90, 0xb6, 0x82, 0xab, 
        0x82, 0xc4, 0x82, 0xa2, 0x82, 0xbd, 0x94, 0xde, 
        0x82, 0xcd, 0x81, 0x41, 0x82, 0xa0, 0x82, 0xe9, 
        0x93, 0xfa, 0x93, 0xcb, 0x91, 0x52, 0x82, 0xcc, 
        0x89, 0xce, 0x8d, 0xd0, 0x82, 0xc5, 0x81, 0x41, 
        0x0a, 0x8f, 0x5a, 0x82, 0xf1, 0x82, 0xc5, 0x82, 
        0xa2, 0x82, 0xbd, 0x83, 0x41, 0x83, 0x70, 0x81, 
        0x5b, 0x83, 0x67, 0x82, 0xf0, 0x8f, 0xc4, 0x82, 
        0xaf, 0x8f, 0x6f, 0x82, 0xb3, 0x82, 0xea, 0x82, 
        0xc4, 0x82, 0xb5, 0x82, 0xdc, 0x82, 0xc1, 0x82, 
        0xbd, 0x81, 0x42, 0x0a, 0x82, 0xbb, 0x82, 0xf1, 
        0x82, 0xc8, 0x94, 0xde, 0x82, 0xc9, 0x81, 0x75, 
        0x82, 0xc6, 0x82, 0xe8, 0x82, 0xa0, 0x82, 0xa6, 
        0x82, 0xb8, 0x82, 0xa4, 0x82, 0xbf, 0x82, 0xc9, 
        0x97, 0x88, 0x82, 0xe9, 0x82, 0xa9, 0x81, 0x48, 
        0x81, 0x76, 0x82, 0xc6, 0x90, 0x65, 0x90, 0xd8, 
        0x82, 0xc8, 0x90, 0xba, 0x82, 0xf0, 0x82, 0xa9, 
        0x82, 0xaf, 0x82, 0xc4, 0x82, 0xad, 0x82, 0xea, 
        0x82, 0xe9, 0x91, 0xe5, 0x89, 0xc6, 0x82, 0xcc, 
        0x96, 0xba, 0x81, 0x41, 0x8b, 0x53, 0x97, 0xb4, 
        0x89, 0x40, 0x83, 0x6e, 0x83, 0x69, 0x81, 0x42, 
        0x0a, 0x89, 0xc2, 0x88, 0xa4, 0x82, 0xa2, 0x8f, 
        0x97, 0x82, 0xcc, 0x8e, 0x71, 0x82, 0xc9, 0x81, 
        0x75, 0x89, 0xc6, 0x82, 0xc9, 0x97, 0x88, 0x82, 
        0xa2, 0x81, 0x76, 0x82, 0xc6, 0x8c, 0xbe, 0x82, 
        0xed, 0x82, 0xea, 0x82, 0xbd, 0x8e, 0x96, 0x82, 
        0xc9, 0x95, 0x40, 0x82, 0xcc, 0x89, 0xba, 0x82, 
        0xf0, 0x90, 0x4c, 0x82, 0xce, 0x82, 0xb5, 0x82, 
        0xc2, 0x82, 0xc2, 0x81, 0x41, 0x82, 0xc2, 0x82, 
        0xa2, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xad, 0x95, 
        0xb6, 0x91, 0xbe, 0x98, 0x59, 0x81, 0x42, 0x0a, 
        0x82, 0xb5, 0x82, 0xa9, 0x82, 0xb5, 0x81, 0x49, 
        0x8f, 0xad, 0x8f, 0x97, 0x82, 0xc9, 0x88, 0xc4, 
        0x93, 0xe0, 0x82, 0xb3, 0x82, 0xea, 0x82, 0xbd, 
        0x90, 0xe6, 0x82, 0xcd, 0x81, 0x41, 0x83, 0x84, 
        0x83, 0x4e, 0x83, 0x55, 0x82, 0xcc, 0x90, 0x65, 
        0x95, 0xaa, 0x82, 0xcc, 0x82, 0xa8, 0x89, 0xae, 
        0x95, 0x7e, 0x82, 0xbe, 0x82, 0xc1, 0x82, 0xbd, 
        0x81, 0x63, 0x82, 0xc1, 0x81, 0x49, 0x0a, 0x0a, 
        0x82, 0xb3, 0x82, 0xb7, 0x82, 0xaa, 0x82, 0xcc, 
        0x95, 0xb6, 0x91, 0xbe, 0x98, 0x59, 0x82, 0xe0, 
        0x8d, 0xc5, 0x8f, 0x89, 0x82, 0xcd, 0x95, 0x7c, 
        0x82, 0xb6, 0x8b, 0x43, 0x82, 0xc3, 0x82, 0xad, 
        0x82, 0xe0, 0x82, 0xcc, 0x82, 0xcc, 0x81, 0x41, 
        0x82, 0xbb, 0x82, 0xcc, 0x82, 0xa8, 0x89, 0xae, 
        0x95, 0x7e, 0x81, 0x75, 0x8b, 0x53, 0x97, 0xb4, 
        0x91, 0x67, 0x81, 0x76, 0x82, 0xaa, 0x8f, 0x97, 
        0x8f, 0x8a, 0x91, 0xd1, 0x82, 0xc6, 0x92, 0x6d, 
        0x82, 0xe9, 0x82, 0xe2, 0x81, 0x41, 0x0a, 0x82, 
        0xa0, 0x82, 0xc1, 0x82, 0xb3, 0x82, 0xe8, 0x82, 
        0xc6, 0x8b, 0x8f, 0x8c, 0xf3, 0x82, 0xf0, 0x8c, 
        0x88, 0x82, 0xdf, 0x8d, 0x9e, 0x82, 0xde, 0x81, 
        0x42, 0x0a, 0x90, 0xdc, 0x82, 0xb5, 0x82, 0xe0, 
        0x81, 0x41, 0x8b, 0x53, 0x97, 0xb4, 0x91, 0x67, 
        0x82, 0xcd, 0x90, 0xe6, 0x91, 0xe3, 0x91, 0x67, 
        0x92, 0xb7, 0x82, 0xf0, 0x96, 0x53, 0x82, 0xad, 
        0x82, 0xb5, 0x82, 0xbd, 0x82, 0xce, 0x82, 0xa9, 
        0x82, 0xe8, 0x82, 0xc5, 0x81, 0x41, 0x93, 0x47, 
        0x91, 0xce, 0x82, 0xb7, 0x82, 0xe9, 0x91, 0x67, 
        0x82, 0xc6, 0x8d, 0x52, 0x91, 0x88, 0x82, 0xcc, 
        0x82, 0xdc, 0x82, 0xc1, 0x82, 0xbd, 0x82, 0xbe, 
        0x92, 0x86, 0x81, 0x42, 0x0a, 0x95, 0xb6, 0x91, 
        0xbe, 0x98, 0x59, 0x82, 0xcd, 0x81, 0x41, 0x89, 
        0xc2, 0x88, 0xa4, 0x82, 0xa2, 0x8f, 0x97, 0x82, 
        0xcc, 0x8e, 0x71, 0x82, 0xc6, 0x82, 0xcc, 0x96, 
        0xb2, 0x82, 0xcc, 0x82, 0xe6, 0x82, 0xa4, 0x82, 
        0xc8, 0x90, 0xb6, 0x8a, 0x88, 0x82, 0xf0, 0x8e, 
        0xe7, 0x82, 0xe9, 0x82, 0xd7, 0x82, 0xad, 0x81, 
        0x41, 0x0a, 0x83, 0x50, 0x83, 0x93, 0x83, 0x4a, 
        0x82, 0xe0, 0x8e, 0xe3, 0x82, 0xa2, 0x82, 0xad, 
        0x82, 0xb9, 0x82, 0xc9, 0x97, 0xa7, 0x82, 0xbf, 
        0x8f, 0xe3, 0x82, 0xaa, 0x82, 0xe9, 0x82, 0xcc, 
        0x82, 0xbe, 0x82, 0xc1, 0x82, 0xbd, 0x81, 0x42, 
        0x0a, 0x0a, 0x0a, 0x81, 0x63, 0x82, 0xc6, 0x82, 
        0xcd, 0x82, 0xa2, 0x82, 0xa6, 0x81, 0x41, 0x83, 
        0x50, 0x83, 0x93, 0x83, 0x4a, 0x82, 0xcd, 0x82, 
        0xa9, 0x82, 0xe7, 0x82, 0xab, 0x82, 0xb5, 0x82, 
        0xcc, 0x95, 0xb6, 0x91, 0xbe, 0x98, 0x59, 0x81, 
        0x42, 0x88, 0xc4, 0x82, 0xcc, 0x92, 0xe8, 0x81, 
        0x41, 0x82, 0xbf, 0x82, 0xf1, 0x82, 0xd2, 0x82, 
        0xe7, 0x8b, 0xa4, 0x82, 0xc9, 0x82, 0xda, 0x82, 
        0xb1, 0x82, 0xda, 0x82, 0xb1, 0x82, 0xc9, 0x82, 
        0xb3, 0x82, 0xea, 0x82, 0xc4, 0x82, 0xb5, 0x82, 
        0xdc, 0x82, 0xa4, 0x81, 0x42, 0x0a, 0x82, 0xbb, 
        0x82, 0xb5, 0x82, 0xc4, 0x81, 0x41, 0x8a, 0xeb, 
        0x8b, 0x40, 0x82, 0xc9, 0x8a, 0xd7, 0x82, 0xc1, 
        0x82, 0xbd, 0x95, 0xb6, 0x91, 0xbe, 0x98, 0x59, 
        0x82, 0xf0, 0x8f, 0x95, 0x82, 0xaf, 0x82, 0xe6, 
        0x82, 0xa4, 0x82, 0xc6, 0x81, 0x41, 0x83, 0x6e, 
        0x83, 0x69, 0x82, 0xaa, 0x8b, 0xec, 0x82, 0xaf, 
        0x8a, 0xf1, 0x82, 0xc1, 0x82, 0xc4, 0x82, 0xab, 
        0x82, 0xbd, 0x8e, 0x9e, 0x81, 0x63, 0x81, 0x63, 
        0x81, 0x42, 0x0a, 0x95, 0xb6, 0x91, 0xbe, 0x98, 
        0x59, 0x82, 0xcc, 0x95, 0xea, 0x82, 0xcc, 0x8c, 
        0x60, 0x8c, 0xa9, 0x82, 0xcc, 0x83, 0x58, 0x83, 
        0x67, 0x83, 0x89, 0x83, 0x62, 0x83, 0x76, 0x82, 
        0xaa, 0x8b, 0x50, 0x82, 0xab, 0x82, 0xf0, 0x95, 
        0xfa, 0x82, 0xbf, 0x81, 0x41, 0x83, 0x6e, 0x83, 
        0x69, 0x82, 0xcd, 0x81, 0x41, 0x81, 0x75, 0x96, 
        0x82, 0x96, 0x40, 0x8f, 0xad, 0x8f, 0x97, 0x81, 
        0x76, 0x82, 0xc9, 0x95, 0xcf, 0x90, 0x67, 0x82, 
        0xb5, 0x82, 0xbd, 0x81, 0x49, 0x81, 0x49, 0x0a, 
        0x5b, 0xd3, 0x54, 0xa9, 0x93, 0x5a, 0x55, 0x21, 
        0x87, 0x11, 0x2d, 0xfe, 0x88, 0x35, 0xe1, 0x44, 
        0x78, 0xad, 0x8a, 0xa9, 0x16, 0x0d, 0xe2, 0x39, 
        0xfe, 0x19, 0x5b, 0x1d, 0xcd, 0x05, 0x1f, 0x67, 
        0xb6, 0x6e, 0x9c, 0xe9, 0x5f, 0x5c, 0xd5, 0x7d, 
        0x84, 0x9b, 0x20, 0x96, 0x35, 0x78, 0xd4, 0xcd, 
        0xb1, 0xf9, 0x93, 0x94, 0xf6, 0x5a, 0x0d, 0x8b, 
        0x7f, 0xe8, 0x76, 0x0b, 0xd8, 0x71, 0x1d, 0x69, 
        0x2d, 0x82, 0xd3, 0xac, 0x5b, 0x02, 0x74, 0xb1, 
        0xa7, 0x0b, 0x81, 0x5a, 0x3b, 0x70, 0x6a, 0x97, 
        0x69, 0xb1, 0xdd, 0x65, 0x03, 0xae, 0x27, 0xee, 
        0xa6, 0x2b, 0x84, 0xa8, 0x03, 0xd6, 0x19, 0xbb, 
        0xf0, 0x1d, 0x51, 0xd9, 0x7f, 0xd1, 0x84, 0xee, 
        0xc7, 0xc5, 0x2a, 0x2d, 0xff, 0x99, 0x7a, 0x10, 
        0x64, 0x0f, 0x05, 0x6f, 0xcb, 0x75, 0xda, 0x3f, 
        0xcc, 0x31, 0x60, 0x84, 0xf3, 0xf6, 0x39, 0x28, 
        0xd3, 0xe0, 0xf1, 0x2a, 0xc6, 0xb2, 0xf2, 0x3a, 
        0xd4, 0x3f, 0x78, 0xfa, 0x37, 0x2e, 0x2b, 0xfc, 
        0xb8, 0x9c, 0xe7, 0xe8, 0x5c, 0xee, 0x8d, 0x2c, 
        0x33, 0x18, 0x28, 0x30, 0x4f, 0xe8, 0x3b, 0x8d, 
        0x82, 0x80, 0x96, 0x71, 0xd4, 0x51, 0x68, 0xbe, 
        0xcf, 0x82, 0x1b, 0xab, 0x5f, 0x70, 0x3d, 0xe2, 
        0xc1, 0xba, 0xa6, 0x0a, 0x49, 0xc3, 0x41, 0xfc, 
        0xe0, 0x57, 0x40, 0x52, 0xd4, 0x17, 0x1d, 0x1a, 
        0xe8, 0x47, 0xa1, 0x50, 0x1e, 0x5b, 0xa6, 0xeb, 
        0xbf, 0xad, 0x59, 0x90, 0xc1, 0x24, 0xb1, 0x0c, 
        0xac, 0x1a, 0x77, 0x91, 0x4b, 0x5d, 0xc7, 0xf5, 
        0x98, 0xe4, 0x1f, 0xe5, 0xa2, 0x41, 0x73, 0x95, 
        0xf6, 0x92, 0x1a, 0xff, 0x5f, 0x56, 0x43, 0xf0, 
        0x3b, 0x72, 0xbc, 0xe1, 0x05, 0x53, 0x9c, 0x6a, 
        0xcd, 0x44, 0x36, 0x4f, 0x6b, 0xa8, 0xc7, 0x33, 
        0x09, 0xcb, 0xe0, 0xf9, 0xef, 0xfa, 0x26, 0x17, 
        0x30, 0x03, 0xb1, 0xbb, 0x51, 0x1a, 0xa5, 0x54, 
        0x44, 0x4a, 0x10, 0x54, 0x0d, 0xe8, 0xb7, 0x0e, 
        0x4b, 0x44, 0x4f, 0x6a, 0x7c, 0xe6, 0xfc, 0x77, 
        0xf2, 0x3f, 0xb2, 0x9f, 0x72, 0xa8, 0x20, 0xfd, 
        0x81, 0xbe, 0x43, 0xc0, 0xc7, 0x51, 0xcb, 0xe2, 
        0x9b, 0x2b, 0x35, 0xa6, 0x16, 0xf6, 0xd3, 0xd1, 
        0x42, 0x68, 0xf6, 0x1c, 0x9d, 0xae, 0xe2, 0x46, 
        0x18, 0x72, 0x89, 0x6d, 0xf7, 0xb7, 0xa0, 0xe7, 
        0x16, 0x0c, 0xf5, 0x65, 0x76, 0xfc, 0x74, 0xa7, 
        0xc5, 0xb5, 0xb2, 0x27, 0x2c, 0x91, 0xfe, 0x7d, 
        0x0f, 0x15, 0xa1, 0xf1, 0xc9, 0xea, 0x8f, 0x65, 
        0x82, 0xdd, 0x9c, 0x5e, 0xe0, 0xd4, 0x4c, 0x1b, 
        0xdc, 0xcc, 0x8e, 0xd0, 0x29, 0x6f, 0x76, 0x54, 
        0x2d, 0x9b, 0x21, 0x2e, 0xf4, 0x91, 0x90, 0x70, 
        0xf6, 0x40, 0x59, 0xbd, 0x3a, 0xd6, 0xe8, 0x73, 
        0x8b, 0x0c, 0x35, 0xd2, 0x90, 0x44, 0x1a, 0x82, 
        0x2c, 0x5d, 0xec, 0xbc, 0x2c, 0x51, 0xf9, 0xc1, 
        0x81, 0x93, 0xdd, 0x10, 0x23, 0xe5, 0xa2, 0x6e, 
        0xcd, 0xf6, 0x7d, 0x12, 0xff, 0x36, 0xd8, 0x9e, 
        0x34, 0x1f, 0x44, 0x2a, 0x00, 0x5a, 0x86, 0xc9, 
        0x9f, 0x78, 0x7e, 0x39, 0x91, 0x9e, 0x78, 0x88, 
        0xb1, 0xa7, 0x08, 0x0b, 0x74, 0x0a, 0x30, 0x3e, 
        0x71, 0x55, 0x6f, 0x2c, 0xfc, 0x0d, 0x9c, 0xeb, 
        0x52, 0xcb, 0x7d, 0x4d, 0xbd, 0x05, 0x90, 0xe1, 
        0x0e, 0xd3, 0x89, 0x6c, 0x3c, 0x57, 0x08, 0x85, 
        0x15, 0x66, 0xd7, 0x40, 0xb4, 0x99, 0x9e, 0xfb, 
        0x4d, 0x12, 0x65, 0x98, 0xa8, 0x47, 0x8b, 0x22, 
        0x7b, 0xd8, 0x15, 0xb6, 0x56, 0xc1, 0xbc, 0x8f, 
        0x3d, 0xba, 0x54, 0x7b, 0x38, 0xbe, 0x79, 0xed, 
        0xc7, 0x70, 0xd5, 0xd4, 0xc8, 0x37, 0x42, 0x15, 
        0xb8, 0xf3, 0x29, 0xe7, 0x10, 0x22, 0xb6, 0xcf, 
        0x4d, 0xd3, 0xa9, 0x63, 0xf1, 0xa2, 0xe0, 0x5c, 
        0x32, 0x23, 0xfc, 0x11, 0x12, 0xad, 0x36, 0x83, 
        0x23, 0x0e, 0x8c, 0x6e, 0x94, 0x24, 0xf3, 0x7c, 
        0xc7, 0x84, 0x19, 0x40, 0x41, 0x1d, 0xcc, 0xa1, 
        0xf4, 0xcb, 0xd8, 0xd6, 0xc5, 0xff, 0xba, 0x95, 
        0xb2, 0x80, 0xb2, 0x3e, 0xdb, 0x8c, 0xfc, 0x63, 
        0x6c, 0x09, 0x5e, 0x4c, 0x93, 0xe0, 0x70, 0x3b, 
        0xd4, 0x53, 0xb8, 0x9b, 0xdf, 0xd2, 0x4a, 0x9e, 
        0x08, 0x74, 0x84, 0xf6, 0x90, 0xb2, 0x57, 0xab, 
        0x6d, 0x0b, 0x68, 0x76, 0xc9, 0xdf, 0x5a, 0xba, 
        0x78, 0x70, 0x91, 0xfd, 0xa3, 0xee, 0xcb, 0xa9, 
        0x20, 0xc6, 0x4c, 0x07, 0x23, 0x18, 0x02, 0xcc, 
        0x60, 0x19, 0xb5, 0x0e, 0x20, 0xea, 0xcd, 0xb3, 
        0x22, 0x92, 0x58, 0xb1, 0x16, 0x36, 0xf6, 0x0a, 
        0xa2, 0xe4, 0xbf, 0xa8, 0x39, 0x29, 0x1a, 0x39, 
        0xae, 0x91, 0x4f, 0x14, 0xc7, 0x1c, 0x5c, 0xc9, 
        0x96, 0x02, 0x06, 0xec, 0x2a, 0xaa, 0xb3, 0x06, 
        0x99, 0xf5, 0x4d, 0x9b, 0xfd, 0x3c, 0x38, 0x9b, 
        0x06, 0x9d, 0x83, 0xc2, 0x48, 0x3c, 0x45, 0x5c, 
        0xc2, 0x41, 0x5e, 0x58, 0x9b, 0xf2, 0x02, 0x83, 
        0xac, 0xd4, 0xd7, 0x63, 0xf9, 0x49, 0x73, 0xc1, 
        0x26, 0xa9, 0xb8, 0x30, 0xe2, 0xdc, 0x1d, 0x75, 
        0x97, 0x63, 0xd0, 0x5a, 0x8d, 0xa8, 0x9a, 0x58, 
        0x09, 0x34, 0xbd, 0xfe, 0x02, 0xef, 0x32, 0x80, 
        0x65, 0x30, 0x99, 0x59, 0xd7, 0xf0, 0x5f, 0xd6, 
        0xbb, 0x39, 0x9c, 0x17, 0xf8, 0x04, 0x76, 0x76, 
        0x69, 0x16, 0x2a, 0x01, 0x45, 0xc3, 0x3d, 0x19, 
        0xf3, 0xa0, 0xe2, 0x01, 0x33, 0x6c, 0x01, 0x47, 
        0xc8, 0x4f, 0x96, 0x88, 0x56, 0xa0, 0x19, 0x48, 
        0xc6, 0xc1, 0xc3, 0x37, 0xbc, 0xfa, 0x97, 0x11, 
        0x69, 0x59, 0x5c, 0xc9, 0x43, 0xb4, 0x51, 0x50, 
        0xb0, 0x3a, 0x05, 0xf4, 0xbb, 0x18, 0xd2, 0x33, 
        0xc3, 0x0e, 0x16, 0x40, 0x89, 0x2b, 0xca, 0xf3, 
        0x4c, 0x24, 0xb8, 0x0a, 0x28, 0x54, 0xda, 0x0c, 
        0xc3, 0x50, 0x71, 0xde, 0x3c, 0x7a, 0x82, 0x6d, 
        0x5a, 0x4d, 0x87, 0x3c, 0x67, 0x73, 0xa3, 0xc6, 
        0x54, 0x4c, 0x3b, 0xd4, 0x18, 0xb7, 0x1e, 0x71, 
        0xe7, 0xc5, 0x1a, 0xf1, 0x4e, 0xff, 0xe8, 0xc0, 
        0x79, 0xc7, 0x8a, 0xb9, 0xfc, 0x15, 0xfc, 0x39, 
        0x99, 0xca, 0xd8, 0x53, 0x5d, 0x55, 0x31, 0x43, 
        0x01, 0xaf, 0x37, 0x98, 0x90, 0xf1, 0x92, 0xa5, 
        0x03, 0x2e, 0x60, 0xd0, 0xa4, 0x3a, 0xd6, 0xdc, 
        0xe1, 0x9c, 0xed, 0xec, 0xd1, 0x48, 0xc0, 0x8d, 
        0x1e, 0xa3, 0xfe, 0x86, 0x12, 0x67, 0x4f, 0x32, 
        0x10, 0x6e, 0x50, 0xc7, 0x93, 0xc5, 0xbc, 0x92, 
        0x61, 0xfe, 0x7f, 0x8c, 0xb4, 0x99, 0x06, 0xd2, 
        0x96, 0xb0, 0x25, 0xdc, 0x1e, 0x6f, 0x34, 0x1b, 
        0x5b, 0x10, 0xd6, 0xb7, 0x13, 0x25, 0xa4, 0xf0, 
        0x9e, 0x56, 0x24, 0x81, 0x43, 0x39, 0x73, 0x94, 
        0x07, 0xd3, 0x46, 0x24, 0x8a, 0x04, 0x62, 0x57, 
        0x84, 0x3b, 0x3b, 0x81, 0xd8, 0x74, 0x94, 0x54, 
        0x97, 0x5c, 0x3b, 0xcc, 0xff, 0xe3, 0x5b, 0xec, 
        0xe9, 0x47, 0xcf, 0x69, 0x5a, 0xe0, 0x22, 0x05, 
        0x4c, 0x25, 0x62, 0x15, 0xf9, 0xe8, 0xd7, 0xc6, 
        0xee, 0xc6, 0xa4, 0xd9, 0xfb, 0xd9, 0x1b, 0xef, 
        0xbd, 0x18, 0xe1, 0x2e, 0x7a, 0xf8, 0x70, 0xea, 
        0x07, 0xb4, 0x46, 0x54, 0x24, 0xe7, 0x59, 0x59, 
        0x5e, 0x6d, 0x5b, 0x63, 0xa5, 0xf2, 0xa3, 0x1d, 
        0xdb, 0x31, 0x54, 0xe2, 0x13, 0xb2, 0x8c, 0x47, 
        0x0c, 0x53, 0x8e, 0xda, 0xeb, 0x7a, 0x7a, 0x73, 
        0x50, 0x27, 0x10, 0xec, 0x71, 0x8c, 0xbd, 0x52, 
        0xaf, 0xb6, 0x36, 0xf5, 0x58, 0x37, 0x6d, 0x9e, 
        0xa2, 0xdd, 0x83, 0x01, 0x46, 0xa9, 0x7c, 0x5c, 
        0x86, 0xc8, 0xcb, 0xa2, 0x0c, 0x19, 0xcc, 0xd2, 
        0x9f, 0x6d, 0xf8, 0x82, 0x25, 0xa4, 0xb4, 0x4c, 
        0x81, 0x35, 0x50, 0x98, 0x61, 0xa0, 0x2e, 0xce, 
        0xf4, 0x70, 0x8f, 0xb1, 0xb7, 0x2a, 0x16, 0xf0, 
        0x35, 0x90, 0x37, 0x6c, 0xbe, 0x96, 0x52, 0xf6, 
        0xe3, 0xd4, 0x5c, 0x26, 0x09, 0xc3, 0x3a, 0xa1, 
        0xbf, 0x9c, 0x9b, 0x1c, 0x8d, 0x57, 0x61, 0x89, 
        0xc1, 0x73, 0xe9, 0xab, 0xa2, 0xf4, 0x46, 0x0b, 
        0x2f, 0x8d, 0x45, 0x70, 0x65, 0x48, 0x4a, 0xf6, 
        0xf1, 0x51, 0xf2, 0x80, 0xfb, 0x05, 0x1f, 0x84, 
        0xa7, 0x66, 0x49, 0xd2, 0xd6, 0x1e, 0x88, 0x6c, 
        0xcf, 0x18, 0xc7, 0x04, 0xd6, 0xb0, 0x8e, 0x08, 
        0xf6, 0x9f, 0x69, 0xfe, 0x66, 0x97, 0xc8, 0xa8, 
        0x63, 0x6e, 0xee, 0x99, 0x0a, 0x58, 0xb7, 0x7b, 
        0xb7, 0xa0, 0xd3, 0xa3, 0xb4, 0xf7, 0xf6, 0x59, 
        0x88, 0x8f, 0xf1, 0x8c, 0xf3, 0xc4, 0x12, 0x06, 
        0xce, 0x01, 0x6f, 0x66, 0x4e, 0x03, 0x72, 0x28, 
        0x10, 0x34, 0x5d, 0x03, 0xc8, 0x4e, 0x5a, 0x8b, 
        0xf7, 0xeb, 0x44, 0xca, 0x23, 0xb9, 0x97, 0xd5, 
        0x2b, 0x4e, 0xd4, 0x2d, 0xf3, 0x9d, 0x90, 0xb6, 
        0x3b, 0xba, 0xb4, 0xfd, 0xfd, 0x4c, 0xb2, 0xe8, 
        0xbc, 0x87, 0xad, 0xcc, 0x72, 0x92, 0x31, 0xa5, 
        0xad, 0x7c, 0x4d, 0x18, 0x46, 0xa2, 0x13, 0x8b, 
        0x39, 0x11, 0x23, 0x35, 0x0d, 0xfc, 0x02, 0xb2, 
        0xa9, 0x2d, 0xaa, 0x2e, 0x95, 0x38, 0x88, 0x9a, 
        0x6b, 0x18, 0x23, 0x1e, 0x11, 0xfb, 0xce, 0xc9, 
        0xf8, 0x9d, 0xb3, 0x2e, 0x11, 0x30, 0x2e, 0xea, 
        0x39, 0x15, 0xc2, 0x74, 0x51, 0x76, 0xfb, 0x13, 
        0x49, 0x0b, 0x1a, 0x35, 0x3f, 0x31, 0xc6, 0xb6, 
        0x61, 0xf2, 0x84, 0xf1, 0x46, 0xf8, 0xc6, 0xec, 
        0x17, 0x2a, 0xfe, 0x46, 0xc0, 0xe8, 0xfc, 0x23, 
        0xc2, 0xbb, 0xb8, 0xbd, 0xa1, 0xa4, 0x61, 0xf9, 
        0x0a, 0xdd, 0xcb, 0xe4, 0x34, 0x85, 0x11, 0xd8, 
        0xce, 0x20, 0x9e, 0xd6, 0x47, 0x9c, 0xed, 0xc7, 
        0x21, 0x47, 0xf9, 0x1b, 0x0d, 0xa1, 0x65, 0xbc, 
        0x86, 0xcb, 0x36, 0xc8, 0x74, 0xe2, 0x9e, 0x17, 
        0x46, 0xad, 0x5b, 0xe1, 0x47, 0xc6, 0xa7, 0xc2, 
        0xef, 0x72, 0x3c, 0x5f, 0x4b, 0xcf, 0x41, 0xb5, 
        0xee, 0x96, 0x84, 0xeb, 0x4c, 0x08, 0x67, 0x00, 
        0x33, 0x50, 0xeb, 0xe6, 0x54, 0x70, 0x9e, 0x74, 
        0xe6, 0x4b, 0xff, 0x66, 0x97, 0x0a, 0x03, 0xf6, 
        0x8b, 0x4c, 0xd4, 0xa0, 0xcf, 0x7c, 0x02, 0xba, 
        0xaa, 0x62, 0xd5, 0x8c, 0x30, 0x9b, 0x40, 0xfb, 
        0x92, 0xd6, 0x8d, 0x22, 0xd0, 0x66, 0xa3, 0x2a, 
        0xdb, 0x19, 0x2b, 0xff, 0xee, 0xea, 0xbb, 0x3d, 
        0x27, 0x28, 0xfd, 0x96, 0xa2, 0x4f, 0xf1, 0x1a, 
        0x57, 0x89, 0xa3, 0xa2, 0x06, 0x2e, 0x47, 0xe0, 
        0x7b, 0x42, 0xd4, 0xe6, 0x08, 0x99, 0xd3, 0xa9, 
        0x57, 0x90, 0x69, 0x4f, 0x18, 0x44, 0xb4, 0x33, 
        0x20, 0x10, 0xd2, 0x50, 0x3b, 0x5d, 0x38, 0x5d, 
        0x3a, 0x9b, 0x7a, 0xec, 0x98, 0xea, 0x25, 0x86, 
        0x9c, 0x47, 0x4f, 0xd6, 0xb6, 0xc7, 0x28, 0x74, 
        0x5f, 0xba, 0x99, 0x7b, 0x94, 0x59, 0xd9, 0x87, 
        0x75, 0xa6, 0x72, 0x1d, 0x91, 0x8e, 0xd3, 0x62, 
        0xeb, 0x3d, 0xc9, 0x78, 0x79, 0xfb, 0x54, 0x06, 
        0x78, 0xf2, 0x6c, 0xb0, 0x93, 0x83, 0xa0, 0xe2, 
        0x20, 0xb7, 0x1b, 0x2c, 0x74, 0x90, 0xe7, 0x3e, 
        0xfd, 0x1d, 0x00, 0x28, 0x9f, 0x3a, 0x40, 0x98, 
        0x58, 0x3a, 0xd7, 0xea, 0x8d, 0x4f, 0xe7, 0xbe, 
        0xd4, 0xa0, 0x74, 0x15, 0x6c, 0xd0, 0x3c, 0x0e, 
        0x19, 0x39, 0xa6, 0xa1, 0x30, 0x53, 0x34, 0xa1, 
        0xcc, 0x3f, 0x73, 0x28, 0x87, 0x91, 0x54, 0xc6, 
        0xb1, 0x2c, 0xea, 0xb8, 0x60, 0x3b, 0xb2, 0x61, 
        0xca, 0x84, 0x2e, 0x29, 0x25, 0x86, 0x51, 0x57, 
        0x0b, 0x89, 0x54, 0x69, 0xd4, 0x4e, 0xd4, 0x3a, 
        0x01, 0xe6, 0xa6, 0xaf, 0xef, 0x26, 0x45, 0xc3, 
        0x7b, 0x88, 0x1f, 0x70, 0x7e, 0x67, 0x9e, 0xb4, 
        0x98, 0xd4, 0xb7, 0x15, 0x24, 0xe3, 0x9a, 0xf0, 
        0x8f, 0xb3, 0x14, 0x2c, 0x6f, 0xc5, 0xf3, 0xb9, 
        0x49, 0x35, 0x4a, 0x8c, 0xfb, 0xcf, 0x51, 0x3d, 
        0x90, 0xc2, 0x26, 0x21, 0x77, 0xef, 0x67, 0xd2, 
        0x51, 0x8b, 0xc0, 0x16, 0x62, 0x67, 0x24, 0x20, 
        0x0f, 0x43, 0xd7, 0x62, 0x24, 0x8c, 0x8f, 0xf3, 
        0x6c, 0x4e, 0x8f, 0xa7, 0x7c, 0xe8, 0xa6, 0x26, 
        0x0c, 0x93, 0x73, 0x5c, 0x02, 0xc0, 0x9d, 0x67, 
        0x49, 0x35, 0x40, 0xbb, 0x71, 0x98, 0x00, 0x02, 
        0x3b, 0x20, 0xe4, 0x99, 0x2c, 0x0e, 0xf9, 0x01, 
        0xad, 0xab, 0x7f, 0x4b, 0x8f, 0xed, 0xd7, 0xb1, 
        0xdf, 0x48, 0xbf, 0x4c, 0x5a, 0x31, 0xe7, 0x77, 
        0xdc, 0xf7, 0x3e, 0xaa, 0x5f, 0xcd, 0x46, 0x45, 
        0x93, 0xfe, 0x15, 0xe4, 0xc1, 0xf9, 0x1c, 0xbe, 
        0x6c, 0x16, 0xb4, 0x47, 0x9b, 0x45, 0xde, 0x38, 
        0x1f, 0x43, 0xd5, 0xea, 0x38, 0x59, 0x2b, 0xc9, 
        0x26, 0x05, 0x1a, 0x0b, 0x56, 0xb4, 0x8d, 0xec, 
        0x72, 0xeb, 0x08, 0xd6, 0x33, 0xcc, 0x9e, 0x59, 
        0x65, 0x32, 0x0d, 0xec, 0x07, 0x94, 0xf5, 0xea, 
        0xb6, 0x78, 0xa5, 0xc7, 0x01, 0x22, 0x68, 0x64, 
        0x99, 0xaa, 0x32, 0x8a, 0xdd, 0x4e, 0xc4, 0x3f, 
        0x70, 0x43, 0xa0, 0x69, 0x34, 0xf0, 0x41, 0xc9, 
        0x70, 0x71, 0xae, 0x21, 0xcf, 0x36, 0x1d, 0xf4, 
        0x4e, 0x72, 0xb5, 0xf6, 0x69, 0xdf, 0xed, 0x28, 
        0x4c, 0x46, 0x3c, 0xb7, 0x16, 0x6f, 0x3c, 0x48, 
        0xfc, 0xbd, 0x51, 0x2d, 0x0d, 0x0e, 0x96, 0x43, 
        0xe2, 0x1d, 0xcc, 0xd2, 0x54, 0xc9, 0x17, 0x1f, 
        0x1c, 0xfe, 0xcb, 0x0e, 0xeb, 0x21, 0x90, 0x07, 
        0xa8, 0x48, 0x02, 0x09, 0xa1, 0x3f, 0x9b, 0x35, 
        0x5a, 0x0e, 0x67, 0x70, 0x09, 0x53, 0x03, 0x23, 
        0x28, 0x82, 0x44, 0xba, 0x0b, 0xcf, 0x3e, 0x31, 
        0x60, 0xec, 0x75, 0x0b, 0x02, 0x01, 0xa9, 0xc6, 
        0xdc, 0x6b, 0xe0, 0x77, 0xe6, 0xbd, 0x0b, 0x0c, 
        0x11, 0xb3, 0xb0, 0xdb, 0xb8, 0x41, 0x06, 0x6b, 
        0xf3, 0xaf, 0x05, 0xc0, 0xeb, 0x70, 0x0a, 0xc8, 
        0x46, 0x31, 0x51, 0xcc, 0xc1, 0x6b, 0x63, 0x40, 
        0xe9, 0x64, 0x89, 0x7d, 0x1d, 0x69, 0x09, 0x51, 
        0xfa, 0x88, 0x9c, 0xd6, 0x2d, 0x2e, 0xd9, 0x5f, 
        0xb4, 0x98, 0xc3, 0x87, 0x02, 0xa6, 0x8e, 0x26, 
        0x3c, 0x59, 0xa4, 0xb6, 0x1c, 0x9e, 0x51, 0xa2, 
        0x26, 0x14, 0x31, 0xac, 0x27, 0x87, 0xa8, 0xa1, 
        0x47, 0x13, 0xc5, 0x5d, 0x8e, 0xc4, 0x52, 0x85, 
        0x46, 0xd8, 0x74, 0x7c, 0xef, 0x72, 0x9c, 0xd0, 
        0xef, 0xee, 0xf9, 0xb0, 0x9e, 0xae, 0x57, 0xaf, 
        0xab, 0x6e, 0x61, 0xe8, 0x66, 0xc0, 0xbb, 0x1b, 
};

static int xcode_building_first_stage(struct cxdec_xcode_status *xcode)
{
      switch (xcode_rand(xcode) % 3) {
      case 2:
        // MOV ESI, EncryptionControlBlock
        // MOV EAX, DWORD PTR DS:[ESI+((xcode_rand(xcode) & 0x3ff) << 2)]
        if (!push_bytexcode(xcode, 0xbe)
                || !push_dwordxcode(xcode, (DWORD)EncryptionControlBlock)
                || !push_2bytesxcode(xcode, 0x8b, 0x86)
                || !push_dwordxcode(xcode, (xcode_rand(xcode) & 0x3ff) << 2))
            return 0;
        break;
    case 0:
          // MOV EAX, EDI
          if (!push_2bytesxcode(xcode, 0x8b, 0xc7))
              return 0;
        break;
      case 1:
        // MOV EAX, xcode_rand(xcode)
        if (!push_bytexcode(xcode, 0xb8)
                || !push_dwordxcode(xcode, xcode_rand(xcode)))
            return 0;
          break;
      }
      return 1;
}

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
    case 6:
        // NOT EAX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd0))
            return 0;
        break;
    case 0:
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
    case 5:
        // DEC EAX
        if (!push_bytexcode(xcode, 0x48))
            return 0;
        break;
    case 4:
        // NEG EAX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd8))
            return 0;
        break;
    case 1:
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
    case 7:
        // INC EAX
        if (!push_bytexcode(xcode, 0x40))
            return 0;
        break;
    case 3:
        // XOR EAX, xcode_rand(xcode)
        if (!push_bytexcode(xcode, 0x35) 
                || !push_dwordxcode(xcode, xcode_rand(xcode)))
            return 0;
        break;
    }
    return 1;
}

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
    case 1:
        // ADD EAX, EBX
        if (!push_2bytesxcode(xcode, 0x01, 0xd8))
            return 0;
        break;
    case 3:
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
    case 4:
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
    case 2:
        // NEG EAX, ADD EAX, EBX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd8) 
                || !push_2bytesxcode(xcode, 0x01, 0xd8))
            return 0;
        break;
    case 5:
        // IMUL EAX, EBX
        if (!push_3bytesxcode(xcode, 0x0f,  0xaf, 0xc3))
            return 0;
        break;
    case 0:
        // SUB EAX, EBX
        if (!push_2bytesxcode(xcode, 0x29, 0xd8))
            return 0;
        break;
    }
    // POP EBX
    return push_bytexcode(xcode, 0x5b);
}

struct cxdec_callback nidaimeTrial_cxdec_callback = {
    "nidaimeTrial",
    { 0x187, 0x7ac },
    xcode_building_stage1
};