#ifndef __flux_correction_h
#define __flux_correction_h

#define MIN_SGRAVITY	500
#define MAX_SGRAVITY	620
#define LITER10_PULSE_NUM	10000	

enum BEJUNG_DEF{B530, B535, B540, B545, B550, B555, B560, B565, B570, B575, B580, B585, B590, B595};
enum FLUX_PARAMETER_DEF{FADJ, FFLUXMETER};


// 유량보정 정보
typedef struct{
	int		rWeight;	// 기준 무게값
	int		tWeight;	// 온도변화따른 무게값
	int		cWeight;	// 보정할 무게값
	int		mp;			// 펄스당 무게
	float	Ratio;		// 10리터 기준 보정비율
	int		rCnt;		// 실제 카운터량
	int		cCnt;		// 보정된 카운터량
}FLUXINFO;

typedef struct{
	long  bLiter;		// 이전값
	long  rLiter;		// 실제량
	long  EventCnt[2];	// 파라메타계산을 위한 이전량과의 차이값
	long  Event[2];		// 각 파라메타에 의한 보정 누적량(증/감)
	float rEvent[2];	// 비율에 의한 보정누적량
	long  Liter;		// 실제량과 보정량의 계산된량(화면 표시량)
}FLUX;

// 유량보정 파라메타 값
typedef struct{
	int		sAdj;			// 설정된 보정량 (gram)
	int		sTemp;			// 기준 온도값
	int		sGravity;		// 기준비중값(환산비중값)
	int		sGravityIndex;	// 적용될 비중index;
	//int	sFluxMeter;		// 유량계보정량

	float	Ratio;			// 10리터기준 보정비율
	float	pFluxMeter;		// 유량계보정비율(%)
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
	short			DispEnb;	// Vfd표시여부
	short			State;      // 충전상태
	short			sFlag;		// 충전시작 Flag
	short			FlowSpeed;	// 초당유속
	short			TempFlag;	// 충전중온도리딩 flag
	short           EncoderErr;	// 엔코드 불량
	short			HallCntErr;	// Hall Sensor Cnt에러
	unsigned long	pLiter;		// PresetLiter
	unsigned long	pAmount;	// PresetAmount
	unsigned long	pStop;      // Preset시 큰밸브 닫힘 Liter
	unsigned long   bLiter;		// 1초전 Cnt량
	unsigned long	rLiter;		// 실제 Cnt량
	unsigned long   hLiter;		// Hall sensor Cnt량
	unsigned long	Liter;		// 보정된 Cnt량(화면 표시량)
	unsigned long	Amount;     // 표시된 금액

	short			Error;		// 에러 저장.
	short			eIndex;		// 에러 발생 횟수
	unsigned long	eLiter;		// 에러발생시 리터량
	unsigned long	rLiter2;	// 전 샘플링 리터량(실제 Cnt량)
	unsigned long	Liter2;		// 전 샘플링 리터량(보정량)

}CHARGESTATE;
void flux_correction_test();
int GetTempIndex(int gtemp);
long flux_correction(long count);
void flux_parameter_set(int stemp,int sgravity,int adj, int ctemp, int fluxmeter);
#endif