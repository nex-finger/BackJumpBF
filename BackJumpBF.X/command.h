/* @file command.h
 * コマンド関連 */

#ifndef COMMAND_H
#define COMMAND_H

void com_putchar(unsigned char in);
void com_puts(unsigned char *in);
void com_response(void);
void com_init(void);
void com_puterr(int inCode);

#endif /* COMMAND_H */
