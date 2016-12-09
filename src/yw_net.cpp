#include "yw.h"


void _NC_STACK_ypaworld::SendCRC(int lvlid)
{
    uint32_t crc = fileCrc32("data:scripts/feinde.scr");
    crc = fileCrc32("data:scripts/feinde.scr", crc);
    crc = fileCrc32("data:scripts/user.scr", crc);
    crc = fileCrc32("data:scripts/weap_f.scr", crc);
    crc = fileCrc32("data:scripts/weap_u.scr", crc);
    crc = fileCrc32("data:scripts/net_robo.scr", crc);
    crc = fileCrc32("data:scripts/net_bldg.scr", crc);
    crc = fileCrc32("data:scripts/flaks.scr", crc);
    crc = fileCrc32("data:scripts/net_ypa.scr", crc);
    crc = fileCrc32("data:scripts/inetrobo.scr", crc);

    char buf[300];
    sprintf(buf, "levels:multi/L%02d%02d.ldf", lvlid, lvlid);

    crc = fileCrc32(buf, crc);

    GameShell->netCRC = crc;

    uamessage_crc crcmsg;
    crcmsg.msgID = UAMSG_CRC;
    crcmsg.owner = GameShell->netPlayerOwner;
    crcmsg.checksum = crc;

    yw_arg181 ywmsg;
    ywmsg.value = &crcmsg;
    ywmsg.val_size = sizeof(crcmsg);
    ywmsg.field_18 = 1;
    ywmsg.field_10 = 0;
    ywmsg.field_14 = 2;

    self_full->ypaworld_func181(&ywmsg);
}
