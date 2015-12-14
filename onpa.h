#ifndef INCLUDED_ONPA
#define INCLUDED_ONPA

extern int onpa_fd2, onpa_fd4, onpa_fd6, onpa_fd8;

void onpa_init();
void onpa_close();

// id は 2, 4, 6, 8のみ対応　シールと対応してるはず
int get_onpa_length(int id);

#endif