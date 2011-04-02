

#define NR_KEYS        88
#define MAX_NR_KEYMAPS 10


#define KM_SHIFT       0
#define KM_CTRL        1
#define KM_CAPS        2

#define K(type,x)     ((x) | (type)<<8)
#define KTYP(x)        ((x)>>8 & 0xff)
#define KVAL(x)		((x) & 0xff)

#define KT_LATIN       0                /* we depend of this value */
#define KT_LETTER      1
#define KT_DEAD        2
#define KT_SHIFT       3
#define KT_FN          4


#define K_LETTER(x)   (K(KT_LETTER, x))
#define K_LATIN(x)    (K(KT_LATIN, x))

#define K_0         K_LATIN('0')
#define K_1         K_LATIN('1')
#define K_2         K_LATIN('2')
#define K_3         K_LATIN('3')
#define K_4         K_LATIN('4')
#define K_5         K_LATIN('5')
#define K_6         K_LATIN('6')
#define K_7         K_LATIN('7')
#define K_8         K_LATIN('8')
#define K_9         K_LATIN('9')

#define K_SCORE    K_LATIN('-')
#define K_EQUAL    K_LATIN('=')
#define K_IBAR     K_LATIN('\\')
#define K_LCLASP   K_LATIN('[')
#define K_RCLASP   K_LATIN(']')
#define K_SEMIC    K_LATIN(';')
#define K_APOST    K_LATIN('\'')
#define K_SQUOTE   K_LATIN('`')
#define K_COMA     K_LATIN(',')
#define K_BAR      K_LATIN('/')
#define K_DOT      K_LATIN('.')
#define K_SPACE    K_LATIN(' ')

#define K_a        K_LETTER('a')
#define K_b        K_LETTER('b')
#define K_c        K_LETTER('c')
#define K_d        K_LETTER('d')
#define K_e        K_LETTER('e')
#define K_f        K_LETTER('f')
#define K_g        K_LETTER('g')
#define K_h        K_LETTER('h')
#define K_i        K_LETTER('i')
#define K_j        K_LETTER('j')
#define K_k        K_LETTER('k')
#define K_l        K_LETTER('l')
#define K_m        K_LETTER('m')
#define K_n        K_LETTER('n')
#define K_o        K_LETTER('o')
#define K_p        K_LETTER('p')
#define K_q        K_LETTER('q')
#define K_r        K_LETTER('r')
#define K_s        K_LETTER('s')
#define K_t        K_LETTER('t')
#define K_u        K_LETTER('u')
#define K_v        K_LETTER('v')
#define K_w        K_LETTER('w')
#define K_x        K_LETTER('x')
#define K_y        K_LETTER('y')
#define K_z        K_LETTER('z')

#define K_A        K_LETTER('A')
#define K_B        K_LETTER('B')
#define K_C        K_LETTER('C')
#define K_D        K_LETTER('D')
#define K_E        K_LETTER('E')
#define K_F        K_LETTER('F')
#define K_G        K_LETTER('G')
#define K_H        K_LETTER('H')
#define K_I        K_LETTER('I')
#define K_J        K_LETTER('J')
#define K_K        K_LETTER('K')
#define K_L        K_LETTER('L')
#define K_M        K_LETTER('M')
#define K_N        K_LETTER('N')
#define K_O        K_LETTER('O')
#define K_P        K_LETTER('P')
#define K_Q        K_LETTER('Q')
#define K_R        K_LETTER('R')
#define K_S        K_LETTER('S')
#define K_T        K_LETTER('T')
#define K_U        K_LETTER('U')
#define K_V        K_LETTER('V')
#define K_W        K_LETTER('W')
#define K_X        K_LETTER('X')
#define K_Y        K_LETTER('Y')
#define K_Z        K_LETTER('Z')

#define K_ESC      K(KT_LATIN, '\033')
#define K_SHIFT    K(KT_SHIFT, KM_SHIFT)
#define K_CTRL     K(KT_SHIFT, KM_CTRL)
#define K_F1       K(KT_FN, 1)
#define K_F2       K(KT_FN, 2)
#define K_F3       K(KT_FN, 3)
#define K_F4       K(KT_FN, 4)
#define K_F5       K(KT_FN, 5)
#define K_F6       K(KT_FN, 6)
#define K_F7       K(KT_FN, 7)
#define K_F8       K(KT_FN, 8)
#define K_F9       K(KT_FN, 9)
#define K_F10      K(KT_FN, 10)



unsigned short plain_map[NR_KEYS] = {
  K_0    ,K_1     ,K_2    ,K_3    ,K_4    ,K_5     ,K_6     ,K_7      ,
  K_8    ,K_9     ,K_SCORE,K_EQUAL,K_IBAR ,K_LCLASP,K_RCLASP,K_SEMIC  ,
  K_APOST,K_SQUOTE,K_COMA ,K_DOT  ,K_BAR  ,0      ,K_a      ,K_b      ,
  K_c    ,K_d     ,K_e    ,K_f    ,K_g    ,K_h    ,K_i      ,K_j      ,
  K_k    ,K_l     ,K_m    ,K_n    ,K_o    ,K_p    ,K_q      ,K_r      ,
  K_s    ,K_t     ,K_u    ,K_v    ,K_w    ,K_x    ,K_y      ,K_z      ,
  K_SHIFT,K_CTRL  ,0      ,0      ,0      ,0      ,0        ,0        ,
  0      ,0       ,K_ESC  ,0      ,0      ,0      ,0        ,0        ,
  K_SPACE,0       ,0      ,0      ,0      ,0      ,0        ,0
};

unsigned short shift_map[NR_KEYS] = {
  0      ,0       ,0      ,0      ,0      ,0      ,0        ,0        ,
  0      ,0       ,0      ,0      ,0      ,0      ,0        ,0        ,
  0      ,0       ,0      ,0      ,0      ,0      ,K_A      ,K_B      ,
  K_C    ,K_D     ,K_E    ,K_F    ,K_G    ,K_H    ,K_I      ,K_J      ,
  K_K    ,K_L     ,K_M    ,K_N    ,K_O    ,K_P    ,K_Q      ,K_R      ,
  K_S    ,K_T     ,K_U    ,K_V    ,K_W    ,K_X    ,K_Y      ,K_Z      ,
  K_SHIFT,K_CTRL  ,0      ,0      ,0      ,0      ,0        ,0        ,
  0      ,0       ,K_ESC  ,0      ,0      ,0      ,0        ,0        ,
  K_SPACE,0       ,0      ,0      ,0      ,0      ,0        ,0
};

unsigned short ctrl_map[NR_KEYS] = {
  0
};


unsigned short ctrl_shift_map[NR_KEYS] = {
  0
};


unsigned short graph_map[NR_KEYS] = {
  0
};

unsigned short ctrl_graph_map[NR_KEYS] = {
  0
};


unsigned char keymaps_count = 7;




unsigned short *key_maps[MAX_NR_KEYMAPS] = {
  plain_map,
  shift_map,
  ctrl_map, ctrl_shift_map,
  graph_map,
  ctrl_graph_map, ctrl_graph_map
};



