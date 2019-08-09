# SN01-AGPS-using-Wifi-Chip-CW02
Code to get GPS data quickly with Wi-Fi chip

Get TOKEN from https://www.u-blox.com/en/solution/services/assistnow, and buld the link:
http://online-live1.services.u-blox.com/GetOnlineData.ashx?token=TOKEN;gnss=gps;datatype=eph,alm,aux,pos;filteronpos;format=aid;lat=LATITUDE;lon=LONGITUDE;pacc=5000

Replace TOKEN, LATITUDE and LONGITUDE with your ublox-TOKEN, current latitude and longitude ( in signed degrees format) respectively. 

and paste this link into code:
    http.begin("http://online-live1.services.u-blox.com/GetOnlineData.ashx?token=TOKEN;gnss=gps;datatype=eph,alm,aux,pos;filteronpos;format=aid;lat=LATITUDE;lon=LONGITUDE;pacc=5000");

