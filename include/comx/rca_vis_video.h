#ifndef _RCA_VIS_VIDEO_H
#define _RCA_VIS_VIDEO_H

unsigned char vis_text_color;
unsigned char vis_text_color_mask;
unsigned long vis_buffer;
#ifdef __CIDELSA__
unsigned char vis_out3_value;
#endif

//rca_vis_video header

void setvideobase(uint16_t vidmem);
void vidclr(uint16_t vidmem, int vidlen);
void vidchar(unsigned short int vidmem, unsigned char character);
void vidstrcpy(uint16_t vidmem, char * text);
unsigned char bgcolor(unsigned char color);
void textcolor(unsigned char color);
#ifdef __TMC600__
void setcolor(uint16_t colormem, unsigned char color);
#else
void shapechar(const unsigned char * shapelocation, unsigned char character, uint16_t color_number);
void shapecolor(uint16_t character, unsigned char number, unsigned char color);
void textcolordefinition(unsigned char definition);
void monochrome(unsigned char mono);
#endif
void rca_vis_video_includer(){
asm(" include comx/rca_vis_video.inc\n");
}

#endif // _RCA_VIS_VIDEO_H
