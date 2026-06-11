/* @file convert.h
 * 入出力の際の変換を担当する */

#ifndef CONVERT_H
#define CONVERT_H

int convertToD(unsigned char inData, unsigned char *outpData);
int convertToX(unsigned char inData, unsigned char *outpData, int inPref);
int convertToB(unsigned char inData, unsigned char *outpData, int inPref);

#endif /* CONVERT_H */