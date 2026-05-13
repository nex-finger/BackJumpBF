/* @file mcp23017.c
 * mcp23017制御ヘッダ */

#ifndef MCP23017_H
#define MCP23017_H

void mcp23017_set(unsigned char inDevice, unsigned char inData);
void mcp23017_translate(unsigned char *outData, unsigned char inData, unsigned char inDot);

#endif /* MCP23017_H */
