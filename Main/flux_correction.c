#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flux_correction.h"
#include "Flux_Table.H"


FLUXINFO		Fi;
//CJPOSINF20	PInf20;
FLUX			flux;
FLUX_PARAMETER	Fp;
CHARGESTATE		Charge;

// �µ����� ������ ����
int GetSGravity(int tempindex)
{
	int diff;

	diff = Fp.sGravity - tWeight_Table[GetTempIndex(Fp.sTemp)][Fp.sGravityIndex];
	return tWeight_Table[tempindex][Fp.sGravityIndex] + diff;
}

// ����� �������� tableIndex ����.
int GetSGravityIndex(int tempindex, int bejung)
{
	int i;
	int low, high, diff1, diff2;

	for(i = B530 + 1; i < B595; i++){
		low = tWeight_Table[tempindex][i-1];
		high =  tWeight_Table[tempindex][i];

		if(low <= bejung && high >= bejung){
			diff1 = bejung - low;
			diff2 = high - bejung;

			if(diff1 < diff2)return i-1;
			else return i;
		}
	}

	if(tWeight_Table[tempindex][B530] > bejung) return B530;
	if(tWeight_Table[tempindex][B595] < bejung) return B595;

	return -1;
}
// �µ�index ����.
int GetTempIndex(int gtemp)
{
    int Tempindex;
	
    Tempindex = gtemp + 50;

    if(Tempindex < 0)Tempindex = 0;
	if(Tempindex > 150)Tempindex = 150;

	return Tempindex;
}

void flux_parameter_set(int stemp,int sgravity,int adj, int ctemp, int fluxmeter,int debug) {

    //int stemp=30; // ���ؿµ�
    //ctemp=60;  // ���� �µ�
    //sgravity=580;  // ���� ����
    //int sGravityIndex;
    int tWeight,rWeight;
    float MP;
    //int adj=0;
    /*
    ���ؿµ��� �о �����ϴ� �κ��� 
        1. FluxrTempSet() �ý��ۼ����� �Լ�������
        2. FluxC_Set(); ADJ ���忡��

    ���غ��ߵ� 
        1.  BejungSet(void)  �ý��ۼ����� �Լ�������   
        2.  FluxC_Set(); ADJ ���忡��  �µ��� �а� ������ ��� �´�. ����.

    ������ ������ �о���� FlowMeterValueCheck(void)/GetFlowMC
        1.  GetFlowMC : ADJ ���忡�� �о�´�. 

    Setup.Data.Adj    : �������� ��(�׷�)     
        2. FluxC_Set(); ADJ ���忡��


    */
    //printf("\n[%s] start �µ�=%d\n",__FUNCTION__,ctemp/2);
    if(debug) printf("\n[%s] start �µ�=%d\n",__FUNCTION__,ctemp*5);
    //memset(&flux, 0, sizeof(FLUX));
    memset(&Fp, 0, sizeof(FLUX_PARAMETER));
    memset(&Fi, 0, sizeof(FLUXINFO));

    // ���� �ٲ��� ���� (����)
    
    Fp.sAdj = adj; // ����������
    Fp.pFluxMeter = 0; //�����躸������ 
    Fp.sGravity = sgravity; // ���غ���

    rWeight = Fp.sGravity * 10;
    Fi.rWeight = rWeight;
        // ���ؿµ�
    Fp.sTemp = stemp;


    // ���غ��߿� ���� ����� ����index (tWeight_Table)
    Fp.sGravityIndex=GetSGravityIndex(GetTempIndex(stemp), sgravity);
    if(debug) printf("\tFp.sGravityIndex=%d\n",Fp.sGravityIndex);  // 8
        // ����µ��� ���� ���߰� (���԰�)
    //tWeight = GetSGravity(GetTempIndex(ctemp)) * 10;

    {
	int diff;
    // ���غ��� - ���ؿµ��� ���߰� ���̺���� ���߰� = �ణ ����
	diff = Fp.sGravity - tWeight_Table[GetTempIndex(Fp.sTemp)][Fp.sGravityIndex];
    if(debug) printf("\tdiff:%d\n",diff);
    // ���� �µ��� ���߰�
	tWeight = (tWeight_Table[GetTempIndex(ctemp)][Fp.sGravityIndex] + diff)*10;
    }



    Fi.tWeight = tWeight;
    // �� �޽��� ����(���� ����)
    MP = (float)((float)(rWeight - Fp.sAdj + (rWeight - tWeight)) / LITER10_PULSE_NUM);
    // 
    Fi.mp = (int)(MP * LITER10_PULSE_NUM);
    if(debug) printf("\tMP=%5.2f Fi.mp=%d\n",MP,Fi.mp);  // 8

    // 10���� ���� ���������� ����.  ���غ��߰� - ���� �µ����߰�
    Fi.cWeight = rWeight - tWeight + (-Fp.sAdj);
    if(Fi.cWeight == 0)Fp.Ratio = 1;
    else {
        // ���� ������ ���� ���� //////////////////////////////////////////////////////////////
        Fp.Ratio = LITER10_PULSE_NUM / ((float)LITER10_PULSE_NUM - ( (float)Fi.cWeight / MP) );
        ///////////////////////////////////////////////////////////////////////////////////////
    }
    Fi.Ratio = Fp.Ratio;
    //////////////////////////////////////////////////////
    if(debug) {
        printf("\tFp.Ratio=%1.3f  rW:%d tW:%d\n",Fp.Ratio,rWeight,tWeight);  // 8
        printf("[%s] end\n",__FUNCTION__);
    }

}

/*

*/
long flux_correction(long count,int debug)
{
    long	DLiter; 
    float	Event;
    int		i;


    flux.rLiter = count;
    DLiter = flux.rLiter - flux.bLiter;
    ////printf(" DLiter:%d\n",(int)DLiter);

    flux.bLiter = flux.rLiter;
    for(i = 0; i <= FFLUXMETER; i++)  //1
        flux.EventCnt[i] += DLiter;

    //if(Fp.Ratio)	// �����Է�X����
    {
        Event = flux.EventCnt[FADJ] * Fp.Ratio;
        if(debug) printf(">>Total=%3.2f diff-input=%lu corrected=%3.2f Total=%3.2f input=%lu\n",
            flux.rEvent[FADJ],
            DLiter,
            Event,
            flux.rEvent[FADJ]+Event,
            count);  // 8

        flux.rEvent[FADJ] += Event;
        flux.EventCnt[FADJ] = 0;

    }

    if(Fp.pFluxMeter) // ������ ����(%)
    {
        ///printf("No more use\n");
        ///Event = flux.EventCnt[FFLUXMETER] * Fp.pFluxMeter;
        //if(Fp.pFluxMeter > 0)Event = -Event;
        ///flux.rEvent[FFLUXMETER] += Event;
        ///flux.EventCnt[FFLUXMETER] = 0;
    }

    flux.Liter = (long)flux.rEvent[FADJ] + (long)flux.rEvent[FFLUXMETER];
    if(flux.Liter < 0)flux.Liter = 0;

    return flux.Liter;

}


void liter_disp(unsigned int liter)
{
	//unsigned int liter;

	Charge.rLiter = liter; //2000; //encoder_counter;  //LiterRead();

#ifndef __BURST_TEST__
#endif

	if(Charge.rLiter2 != Charge.rLiter){

		//FluxSec1 = 0; EnTick = 0;
		liter = flux_correction(Charge.rLiter,0);

#ifndef __BURST_TEST__
#endif

		// Hall ���� ���� üũ ///////////////////////
		//////////////////////////////////////////////

		Charge.Liter2 = liter;

		if(liter > 1 /*DISPLAY_LITER_MIN*/){

			////////////////////////////////////////////////////
			// Preset
			if(Charge.pLiter){

				if(liter >= (Charge.pLiter - 1)){
					//PUMP(OFF); SOL_L(OFF); SOL_S(OFF);
					Charge.State = 0;
					//CHARGE_DISPLAY(OFF);
					Charge.Liter = Charge.pLiter;

				}else Charge.Liter = liter;

				if(Charge.Liter >= Charge.pStop){
					// Preset�� ��� ������� ���� Check ����
					//Setup.Data.SlowCnt = 0;
					//SOL_L(OFF);
				}
			/////////////////////////////////////////////////////
			}else Charge.Liter = liter;

			//SaleDataVfdPut2();
			//Vfd_Display(true);
            //printf("Charge.Liter:%04lu\n",Charge.Liter);
            printf("Charge.Liter:%3.03f\n",(float)Charge.Liter/1000);

#ifdef CHARGING_LITER_SAVE
#endif

			//POS_PULSE_OUT(){ExPosCnt(POINT2_LITER_CUL2(), true);}

		}
	}
	Charge.rLiter2 = Charge.rLiter;
}

void flux_correction_test(){
    //int i;
    //unsigned char pulse=0xff;
    //int toggle=0;
    int liter;
    int corrected_liter;

    flux_parameter_set(30,570,0, -300/5, 0,1);
    flux_parameter_set(30,570,0, -200/5, 0,1);
    flux_parameter_set(30,570,0, -150/5, 0,1);
    flux_parameter_set(30,570,0, -100/5, 0,1);
    flux_parameter_set(30,570,0, -50/5, 0,1);
    flux_parameter_set(30,570,0, 0/5, 0,1);
    flux_parameter_set(30,570,0, 10/5, 0,1);
    flux_parameter_set(30,570,0, 100/5, 0,1);
    flux_parameter_set(30,570,0, 150/5, 0,1);
    flux_parameter_set(30,570,0, 200/5, 0,1);
    flux_parameter_set(30,570,0, 250/5, 0,1);
    flux_parameter_set(30,570,0, 300/5, 0,1);
    flux_parameter_set(30,570,0, 350/5, 0,1);
    printf("------------------------------------------->\n");






    flux_parameter_set(30,570,0, 100/5, 0,1); Fp.Ratio=0.5;
    memset(&flux, 0, sizeof(FLUX));
    //corrected_liter=flux_correction(0,1);



    liter=1000;
    //printf("cnt:%d to +%d\n",liter,liter+50);
    //for(i=liter;i<liter+50;i++)
    corrected_liter=flux_correction(liter,1);
    printf("corrected_liter=%d liter=%d\n",corrected_liter,liter);

    flux_parameter_set(30,570,0, 100/5, 0,1); Fp.Ratio=0.8;

    liter=1100;
    corrected_liter=flux_correction(liter,1);
    printf("corrected_liter=%d liter=%d\n",corrected_liter,liter);

    flux_parameter_set(30,570,0, 100/5, 0,1); Fp.Ratio=0.5;

    liter=1200;
    corrected_liter=flux_correction(liter,1);
    printf("corrected_liter=%d liter=%d\n",corrected_liter,liter);


    //for(i=1000;i<22000;i++)
    //    liter_disp(i);
#if 0
    for(i=0;i<100;i++) {
        printf("pulse %x\n",pulse);
        if(pulse==0xf0) { toggle^=1;  pulse=0xff; }
        if(toggle) { printf("pulse--1 %d\n",toggle);  }
        else { printf("pulse----0 %d\n",toggle); } 
        pulse<<=1;

    }
#endif    

    #if 0
    int balance,temp_index;

    for(i=300;i<=310;i++){
        if(i%10>4) balance =1;
        else balance =0;
        temp_index= (i/10)*2 + balance;
        printf("Temp:%d :%d %d %d\n",i,i/10,i%10,temp_index);

    }

    for(i=300;i<=310;i++){
        printf("Temp:%d :%d \n",i,i/2);
    }
 

    #endif


}