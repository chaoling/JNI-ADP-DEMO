#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdint.h>
#include "_precomp.h"
#include "_fp.h"
#include "_fphlp.h"
#include <dirent.h>
#include <jni.h>
#include "ADP_FPU.h"
#include "ADP_FPU_Light.h"

#define APP_REPEAT		20
#define APP_DEF_FP_NUM	0
#define APP_CAP_TIMEOUT 500
#define JNI_FALSE  0
#define JNI_TRUE   1
#define hlp_printf printf
#define app_printf printf

struct timeval tv1,tv2;
char *templateLocation;
int fd ;
static uint32_t *gpio;
off_t target = 0x56000000;

//////////////////////// local helper function segment //////////////////////////////////////////
void _tick_stat()
{
	gettimeofday(&tv1, NULL);
}

int _tick_end()
{
	gettimeofday(&tv2, NULL);
	return (tv2.tv_sec-tv1.tv_sec)*1000000+(tv2.tv_usec-tv1.tv_usec);
}

void _tick_print(char* szPrompt, int nTime)
{
	app_printf("%s %d.%03ds\n",szPrompt,nTime/1000000,(nTime/1000)%1000);
}
BOOL _capture_finger(int nMilliSecondTimeout)
{
	int nRet;
	struct timeval tv3,tv4;

	gettimeofday(&tv3, NULL);
	while(1)
	{
		_tick_stat();
		nRet = SB_FP_CAPTUREFINGER();
		if(nRet == 0)
		{
			return TRUE;
		}

		gettimeofday(&tv4, NULL);

		if((tv4.tv_sec-tv3.tv_sec)*1000+(tv4.tv_usec-tv3.tv_usec)/1000 > nMilliSecondTimeout)
		{
			//app_printf("Timeout\n");
			break;
		}
	}

	return FALSE;
}


int _memory_setup(){
	size_t free =getpagesize();
	//Obtain handle to physical memory
	if ((fd = open ("/dev/mem", O_RDWR | O_SYNC) ) < 0) {
		printf("Unable to open /dev/mem: %s\n", strerror(errno));
		return -1;
	}
	//map a page of memory to gpio at offset 0x56000000
	gpio = (uint32_t *)mmap(0, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, target );
	if((int32_t)gpio < 0) {
		printf("Mmap failed: %s\n", strerror(errno));
		return -1;
	}
	//gpio = gpio + 0X1C;//Offset of GPH Register base is $target adding 1 to base pointer adds 4 to address in 32 bit memory offset of GPH is 70 0x70/0x4 = 0x1C
	return 0;
}

int _memory_close(){
	close(fd);
	if(munmap(gpio, getpagesize())<0){
		return -1;
	}
	else{
		return 0;
	}
}

char* _ultostr(unsigned long num, char *str, int base)
{
	//hlp_printf("Convert unsigned long number to string..base %d.\n",base);
	unsigned long temp = num;
	unsigned int digit;
	if ( NULL == str || 1 > base ){
		return NULL;
	}
	//Calculate number of digits for the string representation of the number
	do {
		temp /= base;
		str += 1;     //move str_ptr to one digit space right
	} while ( temp > 0 );
	*str = '\0';
	//Now move backwards to fill the digits
	do {
		digit = num-base*(num/base);
		if ( digit < 10 ){
			*(--str) = '0' + digit;
		}
		else {
			*(--str) = 'A'-10 + digit;
		}
		num = num/base;
	} while (num > 0);
	return str;
}

///////////////////// JNI API segment /////////////////////////////////////

JNIEXPORT void JNICALL Java_com_synel_synergy_synergy2416_presentation_controller_FPU_REDON(JNIEnv * env, jclass class){
	printf("Red LED ON\n");
	//	if(_memory_setup() < 0){
	//		printf("Memory Setup Failed Critical Error\n");
	//	}
	//	else{
	//		*gpio = *(gpio) & (~0x10000); //Set RED LED Pin as output
	//		*(gpio+0x1)= *(gpio+0x1) | 0x10000; //RedON-->Synergy2416 GPA16
	//		if(_memory_close() < 0){
	//			printf("Memory Failed to Close-May Cause Memory Leak\n");
	//		}
	//	}
}

JNIEXPORT void JNICALL Java_com_synel_synergy_synergy2416_presentation_controller_FPU_REDOFF(JNIEnv * env, jclass class){
	printf("Red LED OFF\n");
	//	if(_memory_setup() < 0){
	//		printf("Memory Setup Failed Critical Error\n");
	//	}
	//	else{
	//		*gpio = *(gpio) & (~0x10000); //Set RED LED Pin as output
	//		*(gpio+0x1)= *(gpio+0x1) & (~0x10000); //RedOFF-->Synergy2416 GPA16
	//		if(_memory_close() < 0){
	//			printf("Memory Failed to Close-May Cause Memory Leak\n");
	//		}
	//	}
}

JNIEXPORT void JNICALL Java_com_synel_synergy_synergy2416_presentation_controller_FPU_GREENON(JNIEnv * env, jclass class){
	printf("Green LED ON\n");
	//	if(_memory_setup() < 0){
	//		printf("Memory Setup Failed Critical Error\n");
	//	}
	//	else{
	//		*gpio = *(gpio) & (~0x8000); //Set RED GREED Pin as output
	//		*(gpio+0x1)= *(gpio+0x1) | 0x8000; //GREEN ON-->Synergy2416 GPA15
	//		if(_memory_close() < 0){
	//			printf("Memory Failed to Close-May Cause Memory Leak\n");
	//		}
	//	}
}

JNIEXPORT void JNICALL Java_com_synel_synergy_synergy2416_presentation_controller_FPU_GREENOFF(JNIEnv * env, jclass class){
	printf("Green LED OFF\n");
	//	if(_memory_setup() < 0){
	//		printf("Memory Setup Failed Critical Error\n");
	//	}
	//	else{
	//		*gpio = *(gpio) & (~0x8000); //Set GREEN LED Pin as output
	//		*(gpio+0x1)= *(gpio+0x1) & (~0x8000); //GREEN OFF-->Synergy2416 GPA15
	//		if(_memory_close() < 0){
	//			printf("Memory Failed to Close-May Cause Memory Leak\n");
	//		}
	//	}
}

JNIEXPORT jint JNICALL Java_com_synel_synergy_synergy2416_presentation_controller_FPU_FP_1OPENDEVICE
(JNIEnv *env, jclass jcls, jstring JtemplateLoc){
	hlp_printf("%s: called \n",__func__);
	int nSensorType = CMOSTYPE_OV7648;
	//	char *nativeString = "3k";
	int nRet=0;//10k 3k CMOSTYPE_HV7131R CMOSTYPE_EB6048
	//	int nPos = 0;
	templateLocation = (char*)(*env)->GetStringUTFChars(env, JtemplateLoc, 0);
	putenv("HOME=/home/admin");
	hlp_printf("%s: templateLocation is %s, nSensorType (3k,%d) \n",__func__,templateLocation,nSensorType);
	//	nRet=hlpOpen(nativeString, nSensorType);
	//	if(nRet < 0)
	//		{
	//		    hlp_printf("%s: nRet %d, try next nSensorType (10k,%d) \n",__func__,nRet,nSensorType);
	//			nativeString = "10k";
	//			nSensorType = CMOSTYPE_HV7131R;
	//			nRet=hlpOpen(nativeString, nSensorType);
	//			if(nRet >= 0){
	//			hlpLoadFingersDB(templateLocation);
	//			return nRet;
	//			}
	//			hlp_printf("%s: nRet %d, try next nSensorType (10k,%d) \n",__func__,nRet,nSensorType);
	//			nativeString = "10k";
	//			nSensorType = CMOSTYPE_EB6048;
	//			nRet=hlpOpen(nativeString, nSensorType);
	//			if(nRet >= 0){
	//			hlpLoadFingersDB(templateLocation);
	//			return nRet;
	//			}
	//			hlp_printf("%s: nRet %d, try next nSensorType (3k,%d) \n",__func__,nRet,nSensorType);
	//			nativeString = "3k";
	//			nSensorType = CMOSTYPE_HV7131R;
	//			nRet=hlpOpen(nativeString, nSensorType);
	//			if(nRet >= 0){
	//			hlpLoadFingersDB(templateLocation);
	//			return nRet;
	//			}
	//			hlp_printf("%s: nRet %d, try next nSensorType (3k,%d) \n",__func__,nRet,nSensorType);
	//			nativeString = "3k";
	//			nSensorType = CMOSTYPE_EB6048;
	//			nRet=hlpOpen(nativeString, nSensorType);
	//			if(nRet >= 0){
	//			hlpLoadFingersDB(templateLocation);
	//			return nRet;
	//			}
	//			return -1;
	//		}
	hlpLoadFingersDB(templateLocation);
	//	//(*env)->ReleaseStringUTFChars(env, sensorLib, nativeString);
	return nRet;
}

JNIEXPORT jint JNICALL Java_com_synel_synergy_synergy2416_presentation_controller_FPU_FP_1CLOSEDEVICE
(JNIEnv *env, jclass jcls){
	hlp_printf("%s: called \n",__func__);
	//return (int)hlpClose();
	return 0;
}


JNIEXPORT jint JNICALL Java_com_synel_synergy_synergy2416_presentation_controller_FPU_FP_1ENROLE_1EMPLOYEE
(JNIEnv *env, jclass jcls,jstring badge, jint fingernum, jlong timeOut, jlong gapTime, jobject  enrollmentHandler ){
	static char fileName[35];
	int nRet = 0;
	const char *nativeBadge =(*env)->GetStringUTFChars(env, badge, 0);
	long badgeL = atol(nativeBadge);
	DWORD ID;
	long fingerN = (long)fingernum;
	int step = 1;
	jboolean readerError = JNI_FALSE ;
	sprintf(fileName,"%s%d_%d",templateLocation,badgeL, fingerN );
	strcat(fileName,".template");
	hlp_printf("%s: template filename is %s\n",__func__,fileName);

	jclass enrollObj = (*env)->GetObjectClass(env,enrollmentHandler);
	jmethodID methodoFPR = (*env)->GetMethodID(env,enrollObj, "onFingerPrintRead", "(I)V");
	jmethodID methodsSC = (*env)->GetMethodID(env,enrollObj, "setStepCount", "(I)V");
	(*env)->CallVoidMethod(env,enrollmentHandler, methodsSC, 3);

	jmethodID methodoRFF = (*env)->GetMethodID(env,enrollObj, "onReadyForFinger", "(IZ)V");
	(*env)->CallVoidMethod(env,enrollmentHandler, methodoRFF,step,readerError);

	hlp_printf("ID is %d, finger num is %d step %d \n",(int)badgeL,fingernum,step);
	if(step ==1){
		//		if (hlpGetEnrollCount() == gRegMax)
		//		 	 {
		//		 	 readerError = JNI_TRUE;
		//		 	 return -108;
		//		 	 }
		//
		//		hlpSearchID((long*)&ID);
		ID =(DWORD) badgeL ;
		//		if ((nRet = hlpEnrollPrepare(badgeL, fingernum, 0)) < 0)
		//		 	{
		//		 	readerError = JNI_TRUE;
		//		 	return -109;
		//		 	}
		//
		//		if((nRet = SB_FP_ENROLLSTART()) < 0)
		//		 	{
		//		 	readerError = JNI_TRUE;
		//		 	return -103;
		//		 	}
		//
		//		if (!_capture_finger(timeOut))
		//		 	{
		//		 	readerError = JNI_TRUE;
		//		 	return -107;
		//		 	}

		nRet = 0;//SB_FP_ENROLLNTH256(step);

		if (nRet < 0)
		{
			readerError = JNI_TRUE;
			return -103;
		}
		else if(nRet > 0)
		{
			readerError = JNI_TRUE;
			return -106;
		}
		else
		{
			(*env)->CallVoidMethod(env,enrollmentHandler, methodoFPR ,step);
		}
	}
	step++;
	usleep(gapTime*1000);
	(*env)->CallVoidMethod(env,enrollmentHandler, methodoRFF,step,readerError);
	int loop = 0;
	while(loop++ <= 10000);

	//	 if (!_capture_finger(timeOut))
	//		{
	//		 readerError = JNI_TRUE;
	//		 return -107;
	//		}

	nRet = 0;//SB_FP_ENROLLNTH256(step);

	if (nRet < 0)
	{
		readerError = JNI_TRUE;
		return -103;
	}
	else if(nRet > 0)
	{
		readerError = JNI_TRUE;
		return -106;
	}

	if(step<3)
	{
		(*env)->CallVoidMethod(env,enrollmentHandler, methodoFPR ,step);
		step++;
		usleep(gapTime*1000);
		(*env)->CallVoidMethod(env,enrollmentHandler, methodoRFF,step,readerError);
	}

	while(loop-- >= 0);

	//	 if (!_capture_finger(timeOut))
	//		{
	//		 readerError = JNI_TRUE;
	//		 return -107;
	//		}

	nRet = 0;//SB_FP_ENROLLNTH256(step);

	if (nRet < 0)
	{
		readerError = JNI_TRUE;
		return -103;
	}
	else if(nRet > 0)
	{
		readerError = JNI_TRUE;
		return -106;
	}
	else{
		//hlpDelete(badgeL,fingernum);
		return 0;
	}


	//	if((nRet = hlpEnrollEnd(badgeL, fingernum, 0)) < 0)
	//		{
	//			readerError = JNI_TRUE;
	//			return -103;
	//		}

	if(hlpSaveTemplateToFile(fileName, &gFeature))
	{
		return 0;
	}
	else
	{
		readerError = JNI_TRUE;
		return -110;
	}
	//	static char fileName[35];
	//	int nRet = 0;
	//	const char *nativeBadge =(*env)->GetStringUTFChars(env, badge, 0);
	//	long badgeL = atol(nativeBadge);
	//	DWORD ID;
	//	long fingerN = (long)fingernum;
	//	int step = 1;
	//	jboolean readerError = JNI_FALSE ;
	//	sprintf(fileName,"%s%d_%d",templateLocation,badgeL, fingerN );
	//	strcat(fileName,".template");
	//    hlp_printf("%s: template filename is %s\n",__func__,fileName);
	//
	//	jclass enrollObj = (*env)->GetObjectClass(env,enrollmentHandler);
	//	jmethodID methodoFPR = (*env)->GetMethodID(env,enrollObj, "onFingerPrintRead", "(I)V");
	//	jmethodID methodsSC = (*env)->GetMethodID(env,enrollObj, "setStepCount", "(I)V");
	//	(*env)->CallVoidMethod(env,enrollmentHandler, methodsSC, 3);
	//
	//	jmethodID methodoRFF = (*env)->GetMethodID(env,enrollObj, "onReadyForFinger", "(IZ)V");
	//	(*env)->CallVoidMethod(env,enrollmentHandler, methodoRFF,step,readerError);
	//
	//	hlp_printf("ID is %d, finger num is %d step %d \n",(int)badgeL,fingernum,step);
	//	if(step ==1){
	//		if (hlpGetEnrollCount() == gRegMax)
	//		 	 {
	//		 	 readerError = JNI_TRUE;
	//		 	 return -108;
	//		 	 }
	//
	//		hlpSearchID((long*)&ID);
	//		ID =(DWORD) badgeL ;
	//		if ((nRet = hlpEnrollPrepare(badgeL, fingernum, 0)) < 0)
	//		 	{
	//		 	readerError = JNI_TRUE;
	//		 	return -109;
	//		 	}
	//
	//		if((nRet = SB_FP_ENROLLSTART()) < 0)
	//		 	{
	//		 	readerError = JNI_TRUE;
	//		 	return -103;
	//		 	}
	//
	//		if (!_capture_finger(timeOut))
	//		 	{
	//		 	readerError = JNI_TRUE;
	//		 	return -107;
	//		 	}
	//
	//		nRet = SB_FP_ENROLLNTH256(step);
	//
	//		 if (nRet < 0)
	//		 	{
	//		 	readerError = JNI_TRUE;
	//		 	return -103;
	//		 	}
	//		 else if(nRet > 0)
	//		 	{
	//		 	readerError = JNI_TRUE;
	//		 	return -106;
	//		 	}
	//		 else
	//		 	{
	//			(*env)->CallVoidMethod(env,enrollmentHandler, methodoFPR ,step);
	//		 	}
	//		 }
	//	step++;
	//	usleep(gapTime*1000);
	//	(*env)->CallVoidMethod(env,enrollmentHandler, methodoRFF,step,readerError);
	//
	//	 while(SB_FP_ISPRESSFINGER() == 0);
	//
	//	 if (!_capture_finger(timeOut))
	//		{
	//		 readerError = JNI_TRUE;
	//		 return -107;
	//		}
	//
	//	nRet = SB_FP_ENROLLNTH256(step);
	//
	//	if (nRet < 0)
	//		{
	//			readerError = JNI_TRUE;
	//			return -103;
	//		}
	//	else if(nRet > 0)
	//		{
	//			readerError = JNI_TRUE;
	//			return -106;
	//		}
	//
	//	if(step<3)
	//		{
	//		(*env)->CallVoidMethod(env,enrollmentHandler, methodoFPR ,step);
	//		step++;
	//		usleep(gapTime*1000);
	//		(*env)->CallVoidMethod(env,enrollmentHandler, methodoRFF,step,readerError);
	//		}
	//
	//	 while(SB_FP_ISPRESSFINGER() == 0);
	//
	//	 if (!_capture_finger(timeOut))
	//		{
	//		 readerError = JNI_TRUE;
	//		 return -107;
	//		}
	//
	//	nRet = SB_FP_ENROLLNTH256(step);
	//
	//	if (nRet < 0)
	//		{
	//			readerError = JNI_TRUE;
	//			return -103;
	//		}
	//	else if(nRet > 0)
	//		{
	//			readerError = JNI_TRUE;
	//			return -106;
	//		}
	//	else{
	//			hlpDelete(badgeL,fingernum);
	//		}
	//
	//
	//	if((nRet = hlpEnrollEnd(badgeL, fingernum, 0)) < 0)
	//		{
	//			readerError = JNI_TRUE;
	//			return -103;
	//		}
	//
	//	if(hlpSaveTemplateToFile(fileName, &gFeature))
	//		{
	//			return 0;
	//		}
	//	else
	//		{
	//			readerError = JNI_TRUE;
	//			return -110;
	//		}

}

JNIEXPORT jint JNICALL Java_com_synel_synergy_synergy2416_presentation_controller_FPU_FP_1VALIDATE_1EMPLOYEE
(JNIEnv *env, jclass jcls,jstring badge, jint fingernum, jlong timeOut ){
	hlp_printf("%s: called \n",__func__);
	const char *nativeBadge =(*env)->GetStringUTFChars(env, badge, 0);
	long badgeL = atol(nativeBadge);
	int nRet=0;
	DWORD ID, dwTotalTime = 0;
	int i=0;
	long fingerNumber = (long)fingernum;

	//	 if (hlpGetEnrollCount() == 0)
	//		{
	//			return -101;
	//		}

	ID = (DWORD)badge;

	//	 if ((nRet = hlpCheckFingerNum(badgeL, fingerNumber)) < 0)
	//		{
	//		 	 return -102;
	//		}
	while(i++ < 99999);

	//	 while(SB_FP_ISPRESSFINGER() == 0);
	//
	//	 if (!_capture_finger(timeOut)){
	//				return -103;
	//		}
	//	  nRet = hlpVerify((long)badgeL, fingerNumber);
	//	  dwTotalTime += (DWORD)_tick_end();
	//	  if (nRet < 0)
	//		  return -104;

	return nRet;

}


JNIEXPORT jint JNICALL Java_com_synel_synergy_synergy2416_presentation_controller_FPU_FP_1DELETE_1TEMPLATE(JNIEnv *env, jclass jcls,jstring badge, jint fingerNum ){
	const char *nativeBadge =(*env)->GetStringUTFChars(env, badge, 0);
	static char fileName[35];
	long badgeL = atol(nativeBadge);
	long fingernum = (long)fingerNum;
	long nRet = 0;
	DWORD ID, confirm;
	hlp_printf("%s: called \n",__func__);
	//	if (hlpGetEnrollCount() == 0)
	//		{
	//			return -101;//NO Templates Loaded
	//		}
	//	hlpSearchID((long*)&ID);
	if( fingernum < 0){
		//		if((nRet = hlpDeleteID(badgeL))< 0){
		//			return -111;//NO TEMPLATE FOR GIVEN USER
		//		}
		//else{
		sprintf(fileName,"rm %s%d_*",templateLocation,badgeL, fingerNum );
		strcat(fileName,".template");
		system(fileName);
		return 0;
		//}
	}
	//	if ((nRet = hlpDelete(badgeL, fingernum)) < 0)
	//	{
	//		return -102;//NO TEMPLATE FOR GIVEN USER
	//	}
	sprintf(fileName,"rm %s%d_%d",templateLocation,badgeL, fingerNum );
	strcat(fileName,".template");
	system(fileName);
	return 0;

}

JNIEXPORT jobjectArray JNICALL Java_com_synel_synergy_synergy2416_presentation_controller_FPU_FP_1GET_1BADGES(JNIEnv *env, jclass jcls){
	int i=0;
	long idsLength = hlpGetEnrollCount();
	DWORD* ids = NULL;
	char* badgeString = malloc(sizeof(DWORD));

	jobjectArray badges = (*env)->NewObjectArray(env, idsLength, (*env)->FindClass(env,"java/lang/String"), (*env)->NewStringUTF(env,""));
	ids = hlpIdIterator();
	if (NULL == ids){
		hlp_printf("error! no badges found!\n");
		return NULL;
	}
	while ( i < idsLength){
		_ultostr(ids[i],badgeString,10);
		(*env)->SetObjectArrayElement(env,badges, i, (*env)->NewStringUTF(env,badgeString));
		i++;
	}
	free(badgeString);
	return badges;
}
JNIEXPORT jint JNICALL Java_com_synel_synergy_synergy2416_presentation_controller_FPU_FP_1GET_1BADGE_1STATUS(JNIEnv *env, jclass jcls,jstring badge, jint fingernum ){
	const char *nativeBadge =(*env)->GetStringUTFChars(env, badge, 0);
	long badgeL = atol(nativeBadge);
	long fingerNumber = (long)fingernum;
	int nRet = 0;
	hlp_printf("%s: badgeL is %d and fingerNumber is %d \n",__func__,badgeL,fingerNumber);
	//	if ((nRet = (int)hlpCheckFingerNum(badgeL, fingerNumber)) < 0){
	//			 	 return -102;
	//	}
	return 0;
}

JNIEXPORT jint JNICALL Java_com_synel_synergy_synergy2416_presentation_controller_FPU_FP_1SET_1TEMPLATE(JNIEnv *env, jclass jcls,jstring badge, jint fingernum, jstring template ){
	static char fileName[50];
	const char *nativeBadge =(*env)->GetStringUTFChars(env, badge, 0);
	const char *nativeTemplate = (*env)->GetStringUTFChars(env, template, 0);
	long badgeL = atol(nativeBadge);
	long fingerNum = (long)fingernum;
	unsigned char * templateDecoded = malloc(sizeof(unsigned char)*1404);

	sprintf(fileName,"%s%d_%d",templateLocation,badgeL, fingerNum );
	strcat(fileName,".template");

	templateDecoded=(unsigned char *)base64_decode(nativeTemplate,1404);
	if(hlpSaveTemplateToFile(fileName, templateDecoded)){
		//		if(hlpSetFpDataOne(badgeL,fingerNum,templateDecoded)){
		//			unlink(fileName);
		//			free(templateDecoded);
		//			return -100;
		//		}
		free(templateDecoded);
		return 0;
	}
	else{
		unlink(fileName);
		free(templateDecoded);
		return -110;
	}

}

JNIEXPORT jstring JNICALL Java_com_synel_synergy_synergy2416_presentation_controller_FPU_FP_1GET_1TEMPLATE(JNIEnv *env, jclass jcls,jstring badge, jint fingernum ){
	const char *nativeBadge =(*env)->GetStringUTFChars(env, badge, 0);
	long badgeL = atol(nativeBadge);
	long fingerNum = (long)fingernum;
	char template[FPDATASIZE];
	char Template2[] = "ZQAAAAABASYAAAAAAAAAAAAAAAAAAAAAFVVUAABqqqQAAD//9AAAv//5AAH///kAAf//+AAB//P8AAH/4fQAAf//9AAB/8PwAAG/0/AAAG//0AAAVqqAAABFVQAAAAAAAAAAAAAAAAAAAAAAEBD7+/sM+/v7+/v/+/v7EPsQ+/v7+xD7+/sQ+wAQ/Pv7EPv7EBD7Bvv7+/v7DvsQEPsA+/v8+/0Q+/v/EBAQ+/78/QD7+/sPEPsQ+/v7+/z7DPv7+/z8CPv7+/v7+xD7+xD7EPv7EP/7+/v7+xD7EPsQEPz7+/sO+/v7+/v7EPv7+/z7+/v7EPsQ+/v7+wEQEPsB+wD8+wz7+/v7BPv9+/v7/hACEPz7+/v7+/z7+wgC+wgQ+/wE+/v7EPsQ+/v7+/v8+/v7+xAJOyf08OTg4eDk8PQnOwkAlAAgABljAwMDBhIF9AQY5w34DPv7E1T8uAA0AD5IAgMC5AsM7vsJ6u/3JhAKFkcdwgBHABsAAwMD8Q7gERPkBQ32LgXpPjH4wQBYAD8WAwMDCPT1FhMG/gH0BQs4DkMNzABbAD8WAgMC9fMMAhEO6BffEhknA00PpQBfADulAwMD8BkE3Pny8vYFJQEQHkUW0wB2AEWHAgICCgPo6hb3ADLX/dwA6TfakAB1ABdKAwMDAAP59PjzBQHxFe48EB0r2wCSACnHAgIC7Pn2Awn68+UY9QwB5y4agABzABsBAgMC9e77+wEDDxDwJfwVAg1PwACOACuQAwMD4OgMGw7tAvsYAP4I8x0awgCcAC+QAwMD5f8G8wAB9u0D/uws8CkQrgCZAC9hAwMDBg7o5vsR99vkEAwrwgvdqACiADMRAwMDDvAA9vMQAe8FAwVV9i7/kQCWADMkAgMC8BPlygbf9Pf1Fdki7REGYwCFACkwAwMD2RD63P/88e4YDvoJ6RY6bAC4AA8lAwMDA+35/Q4Q+voIF/kOPjPgVQCSAAcPAwMD5vgH8Abm5vDpCfzoEi/bUACoAC2JAwMD7PL68AzuE/LrIgMOGxw2RQCSAAVeAwMDDPPt/wYEAAUSIPwGLTDKNwBdAB+6AwMD5xIF6f0FCP3jLv8dKiUuSAA9AD9mAwMD/P/z/wwY8QEKJPoXPzbqWABjAB0dAwMD7gIDy/IEAgz4JPUbEx45GwBJAEMlAgIB9Pz4+hT49OsIHPcCSTDknACYADNvAgMCBiTo0wDz8AADGuE0ACHsYwCXAC0BAgMCAPUD5/4T9fr6HvIQ/g43DACXAAMAAQEAAAAAAAAAAAAAAAAAAAAAFABjACEhAQEB8QQA3QsPAvzuGQgZNCobmwCBABFhAQMB9v33APkVE+TqEiQnu+bktQBvAD8+AQMBDOL2DQ4cFQfnBDok0Tr6xwChAC+QAQMB9f31/fgF/Q4eBd40ARcLaACZAC2JAQMB++v19xH4A/sGHvQW9xg8uwCeAC+8AQMB3eoL5fglCObiAPA40RzmzQA7AD+PAQIB8xvw6QD4/PwQIBPvFkUX2wCdACmzAQIB1+YFGBD58/gL9wUN7S0i4ABGAEE8AQEB9AwD7fv3+AD+JQ4XFU4Z7wBXAEN+AQEBAP83AgoP2vzd6fkZC1X49AB3AEc4AQEBBSPl6g3x8/T699fzCUn4AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	char Template3[] = "ZgAAAAABARsAAAAAAAAAAAAAAAAAAAAABVVAAABaqpAAAGr/kAABr//wAAG///AAAb//+AABv+/8AAH/x/gAAf/f9AAB///wAAH/r/AAAH8P4AAACiqAAAAVUQAAAAAAAAAAAAAAAAAAAAAA/Pz+/BT8/Pz8/Pz8/BT8/Pz8/Pz8FPz8/Pz8/Pz8/Pz8/xQKFP8U/Pz8/Pz8/BT8/Pz8/vz8/Pz8/Pz8FPz8FAwU/AT8APwTFP/+/PwA/Pz8BhT8/Pz8/An8/Pz8/AD8/Pz8/PwU/P38/Pz8/f78/vz8/A/8/An/FPwUBBT8/PwF/Pz8/Pz8FP4U/Pz8/RT8/PwU/P38/P38FPwUCPz8/vz8/AD8ABT8/wf8/Pz8/AUK/BT8/PwC/BD/B/38FPz8/P0U/BT8/PwOPSL27OXh4OHl7PYiPQ4AxwA6ABKSAwMD9wn1+gf7/P7vKgIsHDIb1QBPAA2JAgIC4vHxAQ/x/voIDfYb/x40qABPABU8AwMD8PoG2//xA+f6HQMrDkAloABYABbCAgMC8vj9BAYOCAL2CyQSFEgkvAB5ADskAgIC/Eby2gUC6f0D/ewPAiAXxgCUABcVAwMDGfL+/RwbAfsFCPISLD/WogBqABYiAwMDCRv3ERcFAfYBDRL3ES0JnAC3ABF4AwMDAQD8Aewa/xEFE/8gLTbbkgCdABCbAwMDC/fj/Ar7AQTwI/30Ax64jQDCAA8mAwMD+d4L5Qoa9OoBB+4SJy/WdQBnAEKOAwMD/gICEgf3AvfrIf7wEyy8NwBbAB+mAwMD3x0H4PsJ4/YBKRoBG0IjlwApABiyAwMDCP/eAx4A8vT6DgMiGEUZswAwABU8AwMD9A0A7A7y6Oj0Gv8nHEYVjwBIABk8AwMD9RX05gHy59sEBPMbD0MeJABWAEN8AgIC+RMKEQ4K2fT8BvcXKVLkRwAwABwuAgIC+Qrv/hIJ9xb2IhQOKkUHcwBEAD8EAgMC6QsFAwsQ2Qj4JgT7NUnvxgCKABkNAgMC+v/7/vok5v30AuIsDj3LpwBXADlBAQMB7vv3LSn6APHjJg/uIR/KtgBuADeHAQIB6wkAAhfz8hr2/9sG7hThVQC9AAlSAQMBAPECAPIP8/ETFN4VMkPhiwATABlIAQEB9wUO5vwX5f/0HQcUCD4OqwBZABNlAQMB/uHp+RD0IAMGEBEc8yo9uQBqADXEAQMB9PgHBBP58hDt9t4L6BvYsACBADskAQMB+yXr0/v86BAGG/kADjYUzQB9ABt6AQEB7wD40uQr5wvcGPsZ+yfIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	char Template7[] = "ZQAAAAABASYAAAAAAAAAAAAAAAAAAAAAFVVUAABqqqQAAD//9AAAv//5AAH///kAAf//+AAB//P8AAH/4fQAAf//9AAB/8PwAAG/0/AAAG//0AAAVqqAAABFVQAAAAAAAAAAAAAAAAAAAAAAEBD7+/sM+/v7+/v/+/v7EPsQ+/v7+xD7+/sQ+wAQ/Pv7EPv7EBD7Bvv7+/v7DvsQEPsA+/v8+/0Q+/v/EBAQ+/78/QD7+/sPEPsQ+/v7+/z7DPv7+/z8CPv7+/v7+xD7+xD7EPv7EP/7+/v7+xD7EPsQEPz7+/sO+/v7+/v7EPv7+/z7+/v7EPsQ+/v7+wEQEPsB+wD8+wz7+/v7BPv9+/v7/hACEPz7+/v7+/z7+wgC+wgQ+/wE+/v7EPsQ+/v7+/v8+/v7+xAJOyf08OTg4eDk8PQnOwkAlAAgABljAwMDBhIF9AQY5w34DPv7E1T8uAA0AD5IAgMC5AsM7vsJ6u/3JhAKFkcdwgBHABsAAwMD8Q7gERPkBQ32LgXpPjH4wQBYAD8WAwMDCPT1FhMG/gH0BQs4DkMNzABbAD8WAgMC9fMMAhEO6BffEhknA00PpQBfADulAwMD8BkE3Pny8vYFJQEQHkUW0wB2AEWHAgICCgPo6hb3ADLX/dwA6TfakAB1ABdKAwMDAAP59PjzBQHxFe48EB0r2wCSACnHAgIC7Pn2Awn68+UY9QwB5y4agABzABsBAgMC9e77+wEDDxDwJfwVAg1PwACOACuQAwMD4OgMGw7tAvsYAP4I8x0awgCcAC+QAwMD5f8G8wAB9u0D/uws8CkQrgCZAC9hAwMDBg7o5vsR99vkEAwrwgvdqACiADMRAwMDDvAA9vMQAe8FAwVV9i7/kQCWADMkAgMC8BPlygbf9Pf1Fdki7REGYwCFACkwAwMD2RD63P/88e4YDvoJ6RY6bAC4AA8lAwMDA+35/Q4Q+voIF/kOPjPgVQCSAAcPAwMD5vgH8Abm5vDpCfzoEi/bUACoAC2JAwMD7PL68AzuE/LrIgMOGxw2RQCSAAVeAwMDDPPt/wYEAAUSIPwGLTDKNwBdAB+6AwMD5xIF6f0FCP3jLv8dKiUuSAA9AD9mAwMD/P/z/wwY8QEKJPoXPzbqWABjAB0dAwMD7gIDy/IEAgz4JPUbEx45GwBJAEMlAgIB9Pz4+hT49OsIHPcCSTDknACYADNvAgMCBiTo0wDz8AADGuE0ACHsYwCXAC0BAgMCAPUD5/4T9fr6HvIQ/g43DACXAAMAAQEAAAAAAAAAAAAAAAAAAAAAFABjACEhAQEB8QQA3QsPAvzuGQgZNCobmwCBABFhAQMB9v33APkVE+TqEiQnu+bktQBvAD8+AQMBDOL2DQ4cFQfnBDok0Tr6xwChAC+QAQMB9f31/fgF/Q4eBd40ARcLaACZAC2JAQMB++v19xH4A/sGHvQW9xg8uwCeAC+8AQMB3eoL5fglCObiAPA40RzmzQA7AD+PAQIB8xvw6QD4/PwQIBPvFkUX2wCdACmzAQIB1+YFGBD58/gL9wUN7S0i4ABGAEE8AQEB9AwD7fv3+AD+JQ4XFU4Z7wBXAEN+AQEBAP83AgoP2vzd6fkZC1X49AB3AEc4AQEBBSPl6g3x8/T699fzCUn4AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	char Template8[] = "ZgAAAAABARsAAAAAAAAAAAAAAAAAAAAABVVAAABaqpAAAGr/kAABr//wAAG///AAAb//+AABv+/8AAH/x/gAAf/f9AAB///wAAH/r/AAAH8P4AAACiqAAAAVUQAAAAAAAAAAAAAAAAAAAAAA/Pz+/BT8/Pz8/Pz8/BT8/Pz8/Pz8FPz8/Pz8/Pz8/Pz8/xQKFP8U/Pz8/Pz8/BT8/Pz8/vz8/Pz8/Pz8FPz8FAwU/AT8APwTFP/+/PwA/Pz8BhT8/Pz8/An8/Pz8/AD8/Pz8/PwU/P38/Pz8/f78/vz8/A/8/An/FPwUBBT8/PwF/Pz8/Pz8FP4U/Pz8/RT8/PwU/P38/P38FPwUCPz8/vz8/AD8ABT8/wf8/Pz8/AUK/BT8/PwC/BD/B/38FPz8/P0U/BT8/PwOPSL27OXh4OHl7PYiPQ4AxwA6ABKSAwMD9wn1+gf7/P7vKgIsHDIb1QBPAA2JAgIC4vHxAQ/x/voIDfYb/x40qABPABU8AwMD8PoG2//xA+f6HQMrDkAloABYABbCAgMC8vj9BAYOCAL2CyQSFEgkvAB5ADskAgIC/Eby2gUC6f0D/ewPAiAXxgCUABcVAwMDGfL+/RwbAfsFCPISLD/WogBqABYiAwMDCRv3ERcFAfYBDRL3ES0JnAC3ABF4AwMDAQD8Aewa/xEFE/8gLTbbkgCdABCbAwMDC/fj/Ar7AQTwI/30Ax64jQDCAA8mAwMD+d4L5Qoa9OoBB+4SJy/WdQBnAEKOAwMD/gICEgf3AvfrIf7wEyy8NwBbAB+mAwMD3x0H4PsJ4/YBKRoBG0IjlwApABiyAwMDCP/eAx4A8vT6DgMiGEUZswAwABU8AwMD9A0A7A7y6Oj0Gv8nHEYVjwBIABk8AwMD9RX05gHy59sEBPMbD0MeJABWAEN8AgIC+RMKEQ4K2fT8BvcXKVLkRwAwABwuAgIC+Qrv/hIJ9xb2IhQOKkUHcwBEAD8EAgMC6QsFAwsQ2Qj4JgT7NUnvxgCKABkNAgMC+v/7/vok5v30AuIsDj3LpwBXADlBAQMB7vv3LSn6APHjJg/uIR/KtgBuADeHAQIB6wkAAhfz8hr2/9sG7hThVQC9AAlSAQMBAPECAPIP8/ETFN4VMkPhiwATABlIAQEB9wUO5vwX5f/0HQcUCD4OqwBZABNlAQMB/uHp+RD0IAMGEBEc8yo9uQBqADXEAQMB9PgHBBP58hDt9t4L6BvYsACBADskAQMB+yXr0/v86BAGG/kADjYUzQB9ABt6AQEB7wD40uQr5wvcGPsZ+yfIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	char Template12[] = "ZQAAAAABASYAAAAAAAAAAAAAAAAAAAAAFVVUAABqqqQAAD//9AAAv//5AAH///kAAf//+AAB//P8AAH/4fQAAf//9AAB/8PwAAG/0/AAAG//0AAAVqqAAABFVQAAAAAAAAAAAAAAAAAAAAAAEBD7+/sM+/v7+/v/+/v7EPsQ+/v7+xD7+/sQ+wAQ/Pv7EPv7EBD7Bvv7+/v7DvsQEPsA+/v8+/0Q+/v/EBAQ+/78/QD7+/sPEPsQ+/v7+/z7DPv7+/z8CPv7+/v7+xD7+xD7EPv7EP/7+/v7+xD7EPsQEPz7+/sO+/v7+/v7EPv7+/z7+/v7EPsQ+/v7+wEQEPsB+wD8+wz7+/v7BPv9+/v7/hACEPz7+/v7+/z7+wgC+wgQ+/wE+/v7EPsQ+/v7+/v8+/v7+xAJOyf08OTg4eDk8PQnOwkAlAAgABljAwMDBhIF9AQY5w34DPv7E1T8uAA0AD5IAgMC5AsM7vsJ6u/3JhAKFkcdwgBHABsAAwMD8Q7gERPkBQ32LgXpPjH4wQBYAD8WAwMDCPT1FhMG/gH0BQs4DkMNzABbAD8WAgMC9fMMAhEO6BffEhknA00PpQBfADulAwMD8BkE3Pny8vYFJQEQHkUW0wB2AEWHAgICCgPo6hb3ADLX/dwA6TfakAB1ABdKAwMDAAP59PjzBQHxFe48EB0r2wCSACnHAgIC7Pn2Awn68+UY9QwB5y4agABzABsBAgMC9e77+wEDDxDwJfwVAg1PwACOACuQAwMD4OgMGw7tAvsYAP4I8x0awgCcAC+QAwMD5f8G8wAB9u0D/uws8CkQrgCZAC9hAwMDBg7o5vsR99vkEAwrwgvdqACiADMRAwMDDvAA9vMQAe8FAwVV9i7/kQCWADMkAgMC8BPlygbf9Pf1Fdki7REGYwCFACkwAwMD2RD63P/88e4YDvoJ6RY6bAC4AA8lAwMDA+35/Q4Q+voIF/kOPjPgVQCSAAcPAwMD5vgH8Abm5vDpCfzoEi/bUACoAC2JAwMD7PL68AzuE/LrIgMOGxw2RQCSAAVeAwMDDPPt/wYEAAUSIPwGLTDKNwBdAB+6AwMD5xIF6f0FCP3jLv8dKiUuSAA9AD9mAwMD/P/z/wwY8QEKJPoXPzbqWABjAB0dAwMD7gIDy/IEAgz4JPUbEx45GwBJAEMlAgIB9Pz4+hT49OsIHPcCSTDknACYADNvAgMCBiTo0wDz8AADGuE0ACHsYwCXAC0BAgMCAPUD5/4T9fr6HvIQ/g43DACXAAMAAQEAAAAAAAAAAAAAAAAAAAAAFABjACEhAQEB8QQA3QsPAvzuGQgZNCobmwCBABFhAQMB9v33APkVE+TqEiQnu+bktQBvAD8+AQMBDOL2DQ4cFQfnBDok0Tr6xwChAC+QAQMB9f31/fgF/Q4eBd40ARcLaACZAC2JAQMB++v19xH4A/sGHvQW9xg8uwCeAC+8AQMB3eoL5fglCObiAPA40RzmzQA7AD+PAQIB8xvw6QD4/PwQIBPvFkUX2wCdACmzAQIB1+YFGBD58/gL9wUN7S0i4ABGAEE8AQEB9AwD7fv3+AD+JQ4XFU4Z7wBXAEN+AQEBAP83AgoP2vzd6fkZC1X49AB3AEc4AQEBBSPl6g3x8/T699fzCUn4AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	char Template13[] = "ZgAAAAABARsAAAAAAAAAAAAAAAAAAAAABVVAAABaqpAAAGr/kAABr//wAAG///AAAb//+AABv+/8AAH/x/gAAf/f9AAB///wAAH/r/AAAH8P4AAACiqAAAAVUQAAAAAAAAAAAAAAAAAAAAAA/Pz+/BT8/Pz8/Pz8/BT8/Pz8/Pz8FPz8/Pz8/Pz8/Pz8/xQKFP8U/Pz8/Pz8/BT8/Pz8/vz8/Pz8/Pz8FPz8FAwU/AT8APwTFP/+/PwA/Pz8BhT8/Pz8/An8/Pz8/AD8/Pz8/PwU/P38/Pz8/f78/vz8/A/8/An/FPwUBBT8/PwF/Pz8/Pz8FP4U/Pz8/RT8/PwU/P38/P38FPwUCPz8/vz8/AD8ABT8/wf8/Pz8/AUK/BT8/PwC/BD/B/38FPz8/P0U/BT8/PwOPSL27OXh4OHl7PYiPQ4AxwA6ABKSAwMD9wn1+gf7/P7vKgIsHDIb1QBPAA2JAgIC4vHxAQ/x/voIDfYb/x40qABPABU8AwMD8PoG2//xA+f6HQMrDkAloABYABbCAgMC8vj9BAYOCAL2CyQSFEgkvAB5ADskAgIC/Eby2gUC6f0D/ewPAiAXxgCUABcVAwMDGfL+/RwbAfsFCPISLD/WogBqABYiAwMDCRv3ERcFAfYBDRL3ES0JnAC3ABF4AwMDAQD8Aewa/xEFE/8gLTbbkgCdABCbAwMDC/fj/Ar7AQTwI/30Ax64jQDCAA8mAwMD+d4L5Qoa9OoBB+4SJy/WdQBnAEKOAwMD/gICEgf3AvfrIf7wEyy8NwBbAB+mAwMD3x0H4PsJ4/YBKRoBG0IjlwApABiyAwMDCP/eAx4A8vT6DgMiGEUZswAwABU8AwMD9A0A7A7y6Oj0Gv8nHEYVjwBIABk8AwMD9RX05gHy59sEBPMbD0MeJABWAEN8AgIC+RMKEQ4K2fT8BvcXKVLkRwAwABwuAgIC+Qrv/hIJ9xb2IhQOKkUHcwBEAD8EAgMC6QsFAwsQ2Qj4JgT7NUnvxgCKABkNAgMC+v/7/vok5v30AuIsDj3LpwBXADlBAQMB7vv3LSn6APHjJg/uIR/KtgBuADeHAQIB6wkAAhfz8hr2/9sG7hThVQC9AAlSAQMBAPECAPIP8/ETFN4VMkPhiwATABlIAQEB9wUO5vwX5f/0HQcUCD4OqwBZABNlAQMB/uHp+RD0IAMGEBEc8yo9uQBqADXEAQMB9PgHBBP58hDt9t4L6BvYsACBADskAQMB+yXr0/v86BAGG/kADjYUzQB9ABt6AQEB7wD40uQr5wvcGPsZ+yfIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	char Template17[] = "ZQAAAAABASYAAAAAAAAAAAAAAAAAAAAAFVVUAABqqqQAAD//9AAAv//5AAH///kAAf//+AAB//P8AAH/4fQAAf//9AAB/8PwAAG/0/AAAG//0AAAVqqAAABFVQAAAAAAAAAAAAAAAAAAAAAAEBD7+/sM+/v7+/v/+/v7EPsQ+/v7+xD7+/sQ+wAQ/Pv7EPv7EBD7Bvv7+/v7DvsQEPsA+/v8+/0Q+/v/EBAQ+/78/QD7+/sPEPsQ+/v7+/z7DPv7+/z8CPv7+/v7+xD7+xD7EPv7EP/7+/v7+xD7EPsQEPz7+/sO+/v7+/v7EPv7+/z7+/v7EPsQ+/v7+wEQEPsB+wD8+wz7+/v7BPv9+/v7/hACEPz7+/v7+/z7+wgC+wgQ+/wE+/v7EPsQ+/v7+/v8+/v7+xAJOyf08OTg4eDk8PQnOwkAlAAgABljAwMDBhIF9AQY5w34DPv7E1T8uAA0AD5IAgMC5AsM7vsJ6u/3JhAKFkcdwgBHABsAAwMD8Q7gERPkBQ32LgXpPjH4wQBYAD8WAwMDCPT1FhMG/gH0BQs4DkMNzABbAD8WAgMC9fMMAhEO6BffEhknA00PpQBfADulAwMD8BkE3Pny8vYFJQEQHkUW0wB2AEWHAgICCgPo6hb3ADLX/dwA6TfakAB1ABdKAwMDAAP59PjzBQHxFe48EB0r2wCSACnHAgIC7Pn2Awn68+UY9QwB5y4agABzABsBAgMC9e77+wEDDxDwJfwVAg1PwACOACuQAwMD4OgMGw7tAvsYAP4I8x0awgCcAC+QAwMD5f8G8wAB9u0D/uws8CkQrgCZAC9hAwMDBg7o5vsR99vkEAwrwgvdqACiADMRAwMDDvAA9vMQAe8FAwVV9i7/kQCWADMkAgMC8BPlygbf9Pf1Fdki7REGYwCFACkwAwMD2RD63P/88e4YDvoJ6RY6bAC4AA8lAwMDA+35/Q4Q+voIF/kOPjPgVQCSAAcPAwMD5vgH8Abm5vDpCfzoEi/bUACoAC2JAwMD7PL68AzuE/LrIgMOGxw2RQCSAAVeAwMDDPPt/wYEAAUSIPwGLTDKNwBdAB+6AwMD5xIF6f0FCP3jLv8dKiUuSAA9AD9mAwMD/P/z/wwY8QEKJPoXPzbqWABjAB0dAwMD7gIDy/IEAgz4JPUbEx45GwBJAEMlAgIB9Pz4+hT49OsIHPcCSTDknACYADNvAgMCBiTo0wDz8AADGuE0ACHsYwCXAC0BAgMCAPUD5/4T9fr6HvIQ/g43DACXAAMAAQEAAAAAAAAAAAAAAAAAAAAAFABjACEhAQEB8QQA3QsPAvzuGQgZNCobmwCBABFhAQMB9v33APkVE+TqEiQnu+bktQBvAD8+AQMBDOL2DQ4cFQfnBDok0Tr6xwChAC+QAQMB9f31/fgF/Q4eBd40ARcLaACZAC2JAQMB++v19xH4A/sGHvQW9xg8uwCeAC+8AQMB3eoL5fglCObiAPA40RzmzQA7AD+PAQIB8xvw6QD4/PwQIBPvFkUX2wCdACmzAQIB1+YFGBD58/gL9wUN7S0i4ABGAEE8AQEB9AwD7fv3+AD+JQ4XFU4Z7wBXAEN+AQEBAP83AgoP2vzd6fkZC1X49AB3AEc4AQEBBSPl6g3x8/T699fzCUn4AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	char Template18[] = "ZgAAAAABARsAAAAAAAAAAAAAAAAAAAAABVVAAABaqpAAAGr/kAABr//wAAG///AAAb//+AABv+/8AAH/x/gAAf/f9AAB///wAAH/r/AAAH8P4AAACiqAAAAVUQAAAAAAAAAAAAAAAAAAAAAA/Pz+/BT8/Pz8/Pz8/BT8/Pz8/Pz8FPz8/Pz8/Pz8/Pz8/xQKFP8U/Pz8/Pz8/BT8/Pz8/vz8/Pz8/Pz8FPz8FAwU/AT8APwTFP/+/PwA/Pz8BhT8/Pz8/An8/Pz8/AD8/Pz8/PwU/P38/Pz8/f78/vz8/A/8/An/FPwUBBT8/PwF/Pz8/Pz8FP4U/Pz8/RT8/PwU/P38/P38FPwUCPz8/vz8/AD8ABT8/wf8/Pz8/AUK/BT8/PwC/BD/B/38FPz8/P0U/BT8/PwOPSL27OXh4OHl7PYiPQ4AxwA6ABKSAwMD9wn1+gf7/P7vKgIsHDIb1QBPAA2JAgIC4vHxAQ/x/voIDfYb/x40qABPABU8AwMD8PoG2//xA+f6HQMrDkAloABYABbCAgMC8vj9BAYOCAL2CyQSFEgkvAB5ADskAgIC/Eby2gUC6f0D/ewPAiAXxgCUABcVAwMDGfL+/RwbAfsFCPISLD/WogBqABYiAwMDCRv3ERcFAfYBDRL3ES0JnAC3ABF4AwMDAQD8Aewa/xEFE/8gLTbbkgCdABCbAwMDC/fj/Ar7AQTwI/30Ax64jQDCAA8mAwMD+d4L5Qoa9OoBB+4SJy/WdQBnAEKOAwMD/gICEgf3AvfrIf7wEyy8NwBbAB+mAwMD3x0H4PsJ4/YBKRoBG0IjlwApABiyAwMDCP/eAx4A8vT6DgMiGEUZswAwABU8AwMD9A0A7A7y6Oj0Gv8nHEYVjwBIABk8AwMD9RX05gHy59sEBPMbD0MeJABWAEN8AgIC+RMKEQ4K2fT8BvcXKVLkRwAwABwuAgIC+Qrv/hIJ9xb2IhQOKkUHcwBEAD8EAgMC6QsFAwsQ2Qj4JgT7NUnvxgCKABkNAgMC+v/7/vok5v30AuIsDj3LpwBXADlBAQMB7vv3LSn6APHjJg/uIR/KtgBuADeHAQIB6wkAAhfz8hr2/9sG7hThVQC9AAlSAQMBAPECAPIP8/ETFN4VMkPhiwATABlIAQEB9wUO5vwX5f/0HQcUCD4OqwBZABNlAQMB/uHp+RD0IAMGEBEc8yo9uQBqADXEAQMB9PgHBBP58hDt9t4L6BvYsACBADskAQMB+yXr0/v86BAGG/kADjYUzQB9ABt6AQEB7wD40uQr5wvcGPsZ+yfIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	char Template22[] = "ZQAAAAABASYAAAAAAAAAAAAAAAAAAAAAFVVUAABqqqQAAD//9AAAv//5AAH///kAAf//+AAB//P8AAH/4fQAAf//9AAB/8PwAAG/0/AAAG//0AAAVqqAAABFVQAAAAAAAAAAAAAAAAAAAAAAEBD7+/sM+/v7+/v/+/v7EPsQ+/v7+xD7+/sQ+wAQ/Pv7EPv7EBD7Bvv7+/v7DvsQEPsA+/v8+/0Q+/v/EBAQ+/78/QD7+/sPEPsQ+/v7+/z7DPv7+/z8CPv7+/v7+xD7+xD7EPv7EP/7+/v7+xD7EPsQEPz7+/sO+/v7+/v7EPv7+/z7+/v7EPsQ+/v7+wEQEPsB+wD8+wz7+/v7BPv9+/v7/hACEPz7+/v7+/z7+wgC+wgQ+/wE+/v7EPsQ+/v7+/v8+/v7+xAJOyf08OTg4eDk8PQnOwkAlAAgABljAwMDBhIF9AQY5w34DPv7E1T8uAA0AD5IAgMC5AsM7vsJ6u/3JhAKFkcdwgBHABsAAwMD8Q7gERPkBQ32LgXpPjH4wQBYAD8WAwMDCPT1FhMG/gH0BQs4DkMNzABbAD8WAgMC9fMMAhEO6BffEhknA00PpQBfADulAwMD8BkE3Pny8vYFJQEQHkUW0wB2AEWHAgICCgPo6hb3ADLX/dwA6TfakAB1ABdKAwMDAAP59PjzBQHxFe48EB0r2wCSACnHAgIC7Pn2Awn68+UY9QwB5y4agABzABsBAgMC9e77+wEDDxDwJfwVAg1PwACOACuQAwMD4OgMGw7tAvsYAP4I8x0awgCcAC+QAwMD5f8G8wAB9u0D/uws8CkQrgCZAC9hAwMDBg7o5vsR99vkEAwrwgvdqACiADMRAwMDDvAA9vMQAe8FAwVV9i7/kQCWADMkAgMC8BPlygbf9Pf1Fdki7REGYwCFACkwAwMD2RD63P/88e4YDvoJ6RY6bAC4AA8lAwMDA+35/Q4Q+voIF/kOPjPgVQCSAAcPAwMD5vgH8Abm5vDpCfzoEi/bUACoAC2JAwMD7PL68AzuE/LrIgMOGxw2RQCSAAVeAwMDDPPt/wYEAAUSIPwGLTDKNwBdAB+6AwMD5xIF6f0FCP3jLv8dKiUuSAA9AD9mAwMD/P/z/wwY8QEKJPoXPzbqWABjAB0dAwMD7gIDy/IEAgz4JPUbEx45GwBJAEMlAgIB9Pz4+hT49OsIHPcCSTDknACYADNvAgMCBiTo0wDz8AADGuE0ACHsYwCXAC0BAgMCAPUD5/4T9fr6HvIQ/g43DACXAAMAAQEAAAAAAAAAAAAAAAAAAAAAFABjACEhAQEB8QQA3QsPAvzuGQgZNCobmwCBABFhAQMB9v33APkVE+TqEiQnu+bktQBvAD8+AQMBDOL2DQ4cFQfnBDok0Tr6xwChAC+QAQMB9f31/fgF/Q4eBd40ARcLaACZAC2JAQMB++v19xH4A/sGHvQW9xg8uwCeAC+8AQMB3eoL5fglCObiAPA40RzmzQA7AD+PAQIB8xvw6QD4/PwQIBPvFkUX2wCdACmzAQIB1+YFGBD58/gL9wUN7S0i4ABGAEE8AQEB9AwD7fv3+AD+JQ4XFU4Z7wBXAEN+AQEBAP83AgoP2vzd6fkZC1X49AB3AEc4AQEBBSPl6g3x8/T699fzCUn4AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	char Template27[] = "ZQAAAAABASYAAAAAAAAAAAAAAAAAAAAAFVVUAABqqqQAAD//9AAAv//5AAH///kAAf//+AAB//P8AAH/4fQAAf//9AAB/8PwAAG/0/AAAG//0AAAVqqAAABFVQAAAAAAAAAAAAAAAAAAAAAAEBD7+/sM+/v7+/v/+/v7EPsQ+/v7+xD7+/sQ+wAQ/Pv7EPv7EBD7Bvv7+/v7DvsQEPsA+/v8+/0Q+/v/EBAQ+/78/QD7+/sPEPsQ+/v7+/z7DPv7+/z8CPv7+/v7+xD7+xD7EPv7EP/7+/v7+xD7EPsQEPz7+/sO+/v7+/v7EPv7+/z7+/v7EPsQ+/v7+wEQEPsB+wD8+wz7+/v7BPv9+/v7/hACEPz7+/v7+/z7+wgC+wgQ+/wE+/v7EPsQ+/v7+/v8+/v7+xAJOyf08OTg4eDk8PQnOwkAlAAgABljAwMDBhIF9AQY5w34DPv7E1T8uAA0AD5IAgMC5AsM7vsJ6u/3JhAKFkcdwgBHABsAAwMD8Q7gERPkBQ32LgXpPjH4wQBYAD8WAwMDCPT1FhMG/gH0BQs4DkMNzABbAD8WAgMC9fMMAhEO6BffEhknA00PpQBfADulAwMD8BkE3Pny8vYFJQEQHkUW0wB2AEWHAgICCgPo6hb3ADLX/dwA6TfakAB1ABdKAwMDAAP59PjzBQHxFe48EB0r2wCSACnHAgIC7Pn2Awn68+UY9QwB5y4agABzABsBAgMC9e77+wEDDxDwJfwVAg1PwACOACuQAwMD4OgMGw7tAvsYAP4I8x0awgCcAC+QAwMD5f8G8wAB9u0D/uws8CkQrgCZAC9hAwMDBg7o5vsR99vkEAwrwgvdqACiADMRAwMDDvAA9vMQAe8FAwVV9i7/kQCWADMkAgMC8BPlygbf9Pf1Fdki7REGYwCFACkwAwMD2RD63P/88e4YDvoJ6RY6bAC4AA8lAwMDA+35/Q4Q+voIF/kOPjPgVQCSAAcPAwMD5vgH8Abm5vDpCfzoEi/bUACoAC2JAwMD7PL68AzuE/LrIgMOGxw2RQCSAAVeAwMDDPPt/wYEAAUSIPwGLTDKNwBdAB+6AwMD5xIF6f0FCP3jLv8dKiUuSAA9AD9mAwMD/P/z/wwY8QEKJPoXPzbqWABjAB0dAwMD7gIDy/IEAgz4JPUbEx45GwBJAEMlAgIB9Pz4+hT49OsIHPcCSTDknACYADNvAgMCBiTo0wDz8AADGuE0ACHsYwCXAC0BAgMCAPUD5/4T9fr6HvIQ/g43DACXAAMAAQEAAAAAAAAAAAAAAAAAAAAAFABjACEhAQEB8QQA3QsPAvzuGQgZNCobmwCBABFhAQMB9v33APkVE+TqEiQnu+bktQBvAD8+AQMBDOL2DQ4cFQfnBDok0Tr6xwChAC+QAQMB9f31/fgF/Q4eBd40ARcLaACZAC2JAQMB++v19xH4A/sGHvQW9xg8uwCeAC+8AQMB3eoL5fglCObiAPA40RzmzQA7AD+PAQIB8xvw6QD4/PwQIBPvFkUX2wCdACmzAQIB1+YFGBD58/gL9wUN7S0i4ABGAEE8AQEB9AwD7fv3+AD+JQ4XFU4Z7wBXAEN+AQEBAP83AgoP2vzd6fkZC1X49AB3AEc4AQEBBSPl6g3x8/T699fzCUn4AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

	//	if(hlpGetFpDataOne(badgeL, fingerNum, template)){
	//		char * templateEncoded;
	//		templateEncoded=(unsigned char *)base64_encode(template,1404);
	//		return (*env)->NewStringUTF(env, templateEncoded);
	//	}
	if ( badgeL > 0 ) {
		char * templateEncoded;
		templateEncoded = Template2;
		return (*env)->NewStringUTF(env, templateEncoded);
	}
	return (*env)->NewStringUTF(env, NULL);
}
JNIEXPORT jint JNICALL Java_com_synel_synergy_synergy2416_presentation_controller_FPU_FP_1GET_1ENROLECOUNT(JNIEnv *env, jclass jcls){
	//return (int)hlpGetEnrollCount();
	return 100;
}

JNIEXPORT jint JNICALL Java_com_synel_synergy_synergy2416_presentation_controller_FPU_FP_1IDENTIFY_1EMPLOYEE(JNIEnv *env,jclass jcls){
	int nRet=0;
	DWORD ID, FingerNum, dwTotalTime = 0,cap_time=0;
	int i;
	int loop = 0;
	while(loop++ <= 10000);

	//		if (hlpGetEnrollCount() == 0)
	//		{
	//			return -101;
	//		}
	//		nRet = SB_FP_CAPTUREFINGER();

	//		if(nRet == 0)
	//		{
	//			nRet = hlpIdentify((long*)&ID, (long*)&FingerNum);
	//			return (long) nRet;
	//		}

	if (nRet < 0)
		return -107;
	else
		return nRet;
}


