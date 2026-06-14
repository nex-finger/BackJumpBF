/* @file convert.h
 * 入出力の際の変換を担当する */

#ifndef CONVERT_H
#define CONVERT_H

/* 数値変換 */
int convertToD(unsigned char inData, unsigned char *outpData);
int convertToX(unsigned char inData, unsigned char *outpData, int inPref);
int convertToB(unsigned char inData, unsigned char *outpData, int inPref);

/* 文字列パース */
int strparse(unsigned char *inS, int *outOffset);

/* 文字列基本ライブラリ */
void strcpy(unsigned char *inS, unsigned char *outS);

#endif /* CONVERT_H */