#ifndef __flux_correction_h
#define __flux_correction_h

#define MIN_SGRAVITY	500
#define MAX_SGRAVITY	620
#define LITER10_PULSE_NUM	10000	

enum BEJUNG_DEF{B530, B535, B540, B545, B550, B555, B560, B565, B570, B575, B580, B585, B590, B595};
enum FLUX_PARAMETER_DEF{FADJ, FFLUXMETER};


// �������� ����
typedef struct{
	int		rWeight;	// ���� ���԰�
	int		tWeight;	// �µ���ȭ���� ���԰�
	int		cWeight;	// ������ ���԰�
	int		mp;			// �޽��� ����
	float	Ratio;		// 10���� ���� ��������
	int		rCnt;		// ���� ī���ͷ�
	int		cCnt;		// ������ ī���ͷ�
}FLUXINFO;

typedef struct{
	long  bLiter;		// ������
	long  rLiter;		// ������
	long  EventCnt[2];	// �Ķ��Ÿ����� ���� ���������� ���̰�
	long  Event[2];		// �� �Ķ��Ÿ�� ���� ���� ������(��/��)
	float rEvent[2];	// ������ ���� ����������
	long  Liter;		// �������� �������� ���ȷ�(ȭ�� ǥ�÷�)
}FLUX;

// �������� �Ķ��Ÿ ��
typedef struct{
	int		sAdj;			// ������ ������ (gram)
	int		sTemp;			// ���� �µ���
	int		sGravity;		// ���غ��߰�(ȯ����߰�)
	int		sGravityIndex;	// ����� ����index;
	//int	sFluxMeter;		// �����躸����

	float	Ratio;			// 10���ͱ��� ��������
	float	pFluxMeter;		// �����躸������(%)
}FLUX_PARAMETER;


#define CHARGE_DISPLAY(x)	Charge.dTime.tick = 0; \
							Charge.dTime.time = 20; \
							Charge.DispEnb = x

typedef struct{
	short   tick;
	short	time;
}CTICK;

typedef struct{
	CTICK			dTime;      // Display TIme
	short			DispEnb;	// Vfdǥ�ÿ���
	short			State;      // ��������
	short			sFlag;		// �������� Flag
	short			FlowSpeed;	// �ʴ�����
	short			TempFlag;	// �����߿µ����� flag
	short           EncoderErr;	// ���ڵ� �ҷ�
	short			HallCntErr;	// Hall Sensor Cnt����
	unsigned long	pLiter;		// PresetLiter
	unsigned long	pAmount;	// PresetAmount
	unsigned long	pStop;      // Preset�� ū��� ���� Liter
	unsigned long   bLiter;		// 1���� Cnt��
	unsigned long	rLiter;		// ���� Cnt��
	unsigned long   hLiter;		// Hall sensor Cnt��
	unsigned long	Liter;		// ������ Cnt��(ȭ�� ǥ�÷�)
	unsigned long	Amount;     // ǥ�õ� �ݾ�

	short			Error;		// ���� ����.
	short			eIndex;		// ���� �߻� Ƚ��
	unsigned long	eLiter;		// �����߻��� ���ͷ�
	unsigned long	rLiter2;	// �� ���ø� ���ͷ�(���� Cnt��)
	unsigned long	Liter2;		// �� ���ø� ���ͷ�(������)

}CHARGESTATE;
void flux_correction_test();
int GetTempIndex(int gtemp);
long flux_correction(long count);
void flux_parameter_set(int stemp,int sgravity,int adj, int ctemp, int fluxmeter);
#endif