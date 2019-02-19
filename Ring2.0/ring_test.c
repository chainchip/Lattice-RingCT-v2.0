#include "ring.h"
#include "rand.h"
#include "common.h"

#define MSIZE 2
#define WSIZE 3
#define NSIZE 3

void LRCT_Byte_Test()
{
	poly a, ra;
	uint8_t seed[NEWHOPE_SYMBYTES] = { 0 };
	unsigned char bCof[NEWHOPE_POLYBYTES] = { 0 };
	OQS_randombytes(seed, NEWHOPE_SYMBYTES);
	poly_uniform(&a, seed);
	printf("begin:\n");
	poly_print(&a);
	printf("serial:\n");
	poly_serial(&a);
	poly_print(&a);
	poly_tobytes(bCof, &a);
	printf("ra:\n");
	poly_frombytes(&ra, bCof);
	poly_print(&ra);


}


void LRCT_Setup_Test()
{
	poly A[2], H[2];
	poly S[2];
	poly L[2];
	poly h;
	poly u[3];
	poly c1;
	poly* t[2];
	unsigned char msg[2] = { 0x01, 0x02 };
	int msgLen = 2;
	unsigned char bt[NEWHOPE_POLYBYTES] = { 0 };
	size_t mLen = 2;
	size_t i = 0;
	size_t k = 0;
	int result = 0;
	int w = 2;
	int pai = 1;

	t[0] = (poly *)malloc((3) * sizeof(poly));
	t[1] = (poly *)malloc((3) * sizeof(poly));

	for (i = 0; i < 2; i++)
	{
		for (k = 0; k < 3; k++)
		{
			poly_init(t[i] + k);
		}
		
	}
	LRCT_Setup(A, H, 2);
	LRCT_SampleKey(S, 2);
	LRCT_KeyGen(L, A, S, 2);
	LRCT_SampleKey(S, 2);
	LRCT_KeyGen(L+1, A, S, 2);

	for (k = 0; k < 3; k++)
	{
		OQS_randombytes(bt, NEWHOPE_POLYBYTES);
		poly_frombytes(u + k, bt);
		poly_serial(u + k);
		///poly_print(u+k);
	}
	printf("====================================\n");
	LRCT_SigGen(&c1, t, &h, A, H,S, u, mLen, L, w,pai, msg, msgLen);
	printf("c1\n");
	poly_print(&c1);
	printf("=================\n");
   result = LRCT_SigVer(&c1, t, A, H, mLen, &h, L,w, msg, msgLen);
   if (result ==1)
   {
	   printf("Successful!\n");
   }
   else
   {
	   printf("fail\n");
   }
   free(t[0]);
   free(t[1]);

}

void MIMO_LRCT_Setup_Test()
{
	poly A[MSIZE], H[MSIZE];
	poly SList[MSIZE*NSIZE];
	poly S[MSIZE];
	poly LList[NSIZE*WSIZE];
	poly hList[NSIZE];
	poly c1;
	poly tList[NSIZE*WSIZE*(MSIZE+1)];
	int i, j, k;
	int pai = 2;
	unsigned char msg[2] = { 0x01, 0x02 };
	int msgLen = 2;
	int result = 0;
	MIMO_LRCT_Setup(A, H, MSIZE);
	for ( i = 0; i < NSIZE; i++)
	{
		LRCT_SampleKey(SList + i*MSIZE, MSIZE);
		MIMO_LRCT_KeyGen(LList + i*WSIZE + (pai-1) , A, SList + i * MSIZE, MSIZE);
	}
	for ( i = 0; i < WSIZE; i++)
	{
		if (i != pai-1)
		{
			for (j = 0; j < NSIZE; j++)
			{
				LRCT_SampleKey(S, MSIZE);
				MIMO_LRCT_KeyGen(LList + j* WSIZE + i, A, S, MSIZE);
			}
		}

	}
	MIMO_LRCT_SigGen(&c1, tList, hList, SList, NSIZE, A, H, MSIZE, LList, WSIZE, pai, msg, msgLen);
	result = MIMO_LRCT_SigVer(&c1, tList, hList, NSIZE, A, H, MSIZE, LList, WSIZE, msg, msgLen);
	if (result == 1)
	{
		printf("Successful!\n");
	}
	else
	{
		printf("fail\n");
	}

}
///Öý±Ò£¬»¨±Ò
void LRCT_Spent_Test()
{
	poly A[MSIZE], H[MSIZE];
	poly skPai[MSIZE], pkPai;
	poly ckPai[MSIZE];
	poly* t[WSIZE];
	unsigned char bMessage[4] = { 0x01, 0x02, 0x03, 0x4 };
	size_t msglen = 4;
	IW iw;
	//////////////////
	poly skOA[MSIZE];
	poly pkOA;
	IW iwOA;
	poly ckOA[MSIZE];
	////////////////////
	IW iwList[WSIZE];
	poly skTmp[MSIZE];
	poly pkList[WSIZE];
	poly ckList[WSIZE][MSIZE];
	unsigned char bListMessage[2] = { 0x01, 0x02};
	size_t msgListlen = 2;
	int i = 0;
	///////////////////
	poly L[WSIZE];
	int paiIndex = 1;
	poly c1, h;
	unsigned char bSignMessage[3] = { 0x01, 0x02, 0x03 };
	size_t msgSignlen = 3;
	int result =0;
	size_t k = 0;
	/////////////////
	for ( i = 0; i < WSIZE; i++)
	{
		t[i] = (poly *)malloc((MSIZE+1) * sizeof(poly));
		for (k = 0; k < MSIZE+1; k++)
		{
			poly_init(t[i] + k);
		}
	}
	///////////////////
	LRCT_Setup(A, H, MSIZE);
	LRCT_SampleKey(skPai, MSIZE);
	LRCT_KeyGen(&pkPai, A, skPai, MSIZE);//A*S+0
	LRCT_Mint(&iw, ckPai, &pkPai, A, MSIZE, bMessage, msglen);//A*ck + $
	///
	LRCT_SampleKey(skOA, MSIZE);
	LRCT_KeyGen(&pkOA, A, skOA, MSIZE);//
	//LRCT_Mint(&iwOA, ckOA, &pkOA, A, MSIZE, bMessage, msglen);
	//////
	for( i = 0; i < WSIZE; i++)
	{
		if (i == paiIndex)
		{
			poly_cofcopy(&iwList[i].a, &iw.a);
			poly_cofcopy(&iwList[i].cn, &iw.cn);
		}
		else
		{
			LRCT_SampleKey(skTmp, MSIZE);
			LRCT_KeyGen(pkList + i, A, skTmp, MSIZE);//A*S+0
			LRCT_Mint(iwList + i, ckList[i], pkList + i, A, MSIZE, bListMessage, msgListlen);
		}

	}
	LRCT_Spend(&iwOA, ckOA, &c1, t, &h, L, bSignMessage, msgSignlen, iwList, WSIZE, paiIndex, skPai, ckPai, bMessage, msglen, &pkOA, A, H, MSIZE);
	result = LRCT_Verify(&c1, t, &h, A, H, MSIZE, bSignMessage, msgSignlen, L, WSIZE);
	if (result == 1)
	{
		printf("Successful!\n");
	}
	else
	{
		printf("fail\n");
	}
	for (i = 0; i < WSIZE; i++)
	{
		free(t[i]);
	}
}


void LRCT_Mul_Test()
{
	poly A[2], H[2], H2[2];
	poly h,h1,r;
	poly S[2];
	LRCT_Setup(A, H, 2);
	LRCT_SampleKey(S, 2);

	LRCT_MatrixMulPoly(&h, H, S, 2);


	for (size_t i = 0; i < NEWHOPE_N; i++)
	{
		h.coeffs[i] = coeff_freeze2Q(NEWHOPE_2Q + NEWHOPE_Q - h.coeffs[i] * 2);
	}
	LRCT_ConstMulMatrix(H2, H, 2, 2);
	LRCT_MatrixMulPoly(&h1, H2, S, 2);
	poly_add(&r, &h1, &h);
	poly_print(&r);


}
void LRCT_MatrixMulVect_Test()
{
	poly A[2], H[2];
	LRCT_Setup(A, H, 2);
	uint8_t bt[2] = { 0 };
	bt[0] = 1;
	bt[1] = 2;


}
void LRCT_Lift_Test()
{
	poly A[2], H[2], LA[3], H2[3];
	poly h;
	poly S[2];
	LRCT_SampleKey(S, 2);
	LRCT_Setup(A, H, 2);
	LRCT_MatrixMulPoly(&h, H, S, 2);
	LRCT_Lift(LA, H, &h, 2);
	////////////////////////////////////
	LRCT_ConstMulMatrix(H2, H, 2, 2);
	for (size_t i = 0; i < NEWHOPE_N; i++)
	{
		H2[2].coeffs[i] = coeff_freeze2Q(NEWHOPE_2Q + NEWHOPE_Q - h.coeffs[i] * 2);
	}
	for (size_t i = 0; i < 3; i++)
	{
		if (poly_equal(LA + i, H2 + i) != 1)
		{
			printf("Lift error_%d!\n", i);
		}
	}
}
void LRCT_KDF_Test()
{

}

void LRCT_Com_Test()
{
	IW iw;
	poly ck[2];
	size_t mLen = 2;
	poly  A[2], H[2], sk[2];
	unsigned char bMessage[4] = { 0x01, 0x02, 0x03, 0x4 };
	size_t msglen = 4;
	poly a;
	LRCT_Setup(A, H, mLen);
	LRCT_SampleKey(sk, mLen);
	LRCT_KeyGen(&a, A, sk, mLen);
	printf("public key:");
	poly_print(&a);
	LRCT_Mint(&iw, ck, &a, A, mLen, bMessage, msglen);
	printf("a:\n");
	poly_print(&(iw.a));
	printf("cn:\n");
	poly_print(&(iw.cn));
}
//ntt ±ä»»²âÊÔ
void LRCT_Fun_Test()
{
	uint8_t seed[NEWHOPE_SYMBYTES] = { 0 };
	poly a;
	OQS_randombytes(seed, NEWHOPE_SYMBYTES);
	poly_uniform(&a, seed);
	poly_serial(&a);
	////////////
	printf("begin:\n");
	poly_print(&a);
	//////
	poly_ntt(&a);
	printf("after:\n");
	poly_print(&a);
	////
	poly_invntt(&a);
	printf("recover:\n");
	poly_print(&a);

}
//ÒÆÎ»²âÊÔ
void LRCT_Shift_Test()
{
	poly r, a;
	poly_init(&r);
	poly_init(&a);
	r.coeffs[NEWHOPE_N - 1] = 1;
	r.coeffs[0] = 1;

	poly_ntt(&r);
	poly_shift(&a, &r, 1);
	poly_invntt(&a);

	poly_serial(&a);
	poly_print(&a);
}
void LRCT_ComHom_Test()
{
	unsigned char message[2] = { 0xF0, 0x0F };
	int messLen = 2;
	int messBitLen = messLen*8;
	int i = 0;
	unsigned char bitTmp = 0;
	poly *CKi = (poly *)malloc((MSIZE*(messBitLen)) * sizeof(poly));
	poly *comList = (poly *)malloc(((messBitLen)) * sizeof(poly));
	poly  A[MSIZE], H[MSIZE], sk[MSIZE], ck0[MSIZE], tmpM[MSIZE];
	poly a, r, tmp;

	poly_init(&a);
	poly_init(&tmp);
	poly_init(&r);
	for ( i = 0; i < MSIZE; i++)
	{
		poly_init(ck0 + i);
	}
	LRCT_Setup(A, H, MSIZE);
	LRCT_SampleKey(sk, MSIZE);
	//left
	LRCT_nttCom(&r, A, sk, MSIZE, message, messLen);
	//right
	for (i = 1; i < messBitLen; i++)
	{
		LRCT_SampleKey(CKi + i*MSIZE, MSIZE);
	}

	LRCT_GetCK0(CKi, sk, MSIZE, CKi+MSIZE, messBitLen-1);
	
	for ( i = 0; i < messBitLen; i++)
	{
		LRCT_MatrixShift(tmpM, CKi+i*MSIZE, MSIZE, i);
		LRCT_MatrixAddMatrix(ck0, ck0, tmpM, MSIZE);
	}
	for ( i = 0; i < MSIZE; i++)
	{
		if (poly_equal(ck0 + i, sk + i) != 1)
		{
			printf("error\n");
			break;
		}
	}

	for ( i = 0; i < messLen; i++)
	{
		bitTmp = (message[i] & 0x01);
		LRCT_nttCom(comList + i * 8, A, CKi + (i * 8) * MSIZE, MSIZE, &bitTmp, 1);
		//////////////////////////////
		bitTmp = (message[i] & 0x02)>>1;
		LRCT_nttCom(comList + i * 8 + 1, A, CKi + (i * 8 + 1) * MSIZE, MSIZE, &bitTmp, 1);
		////////////
		bitTmp = (message[i] & 0x04)>>2;
		LRCT_nttCom(comList + i * 8 + 2, A, CKi + (i * 8 + 2) * MSIZE, MSIZE, &bitTmp, 1);
		////////////
		bitTmp = (message[i] & 0x08)>>3;
		LRCT_nttCom(comList + i * 8 + 3, A, CKi + (i * 8 + 3) * MSIZE, MSIZE, &bitTmp, 1);

		////////////
		bitTmp = (message[i] & 0x10)>>4;
		LRCT_nttCom(comList + i * 8 + 4, A, CKi + (i * 8 + 4) * MSIZE, MSIZE, &bitTmp, 1);

		////////////
		bitTmp = (message[i] & 0x20)>>5;
		LRCT_nttCom(comList + i * 8 + 5, A, CKi + (i * 8 + 5) * MSIZE, MSIZE, &bitTmp, 1);

		////////////
		bitTmp = (message[i] & 0x40)>>6;
		LRCT_nttCom(comList + i * 8 + 6, A, CKi + (i * 8 + 6) * MSIZE, MSIZE, &bitTmp, 1);

		////////////
		bitTmp = (message[i] & 0x80)>>7;
		LRCT_nttCom(comList + i * 8 + 7, A, CKi + (i * 8 + 7) * MSIZE, MSIZE, &bitTmp, 1);
	}
	//poly_cofcopy(&a, comList);
	for ( i = 0; i < messBitLen; i++)
	{
		poly_shift(&tmp, comList + i, i);
		poly_add(&a, &a, &tmp);
	}
	printf("a:\n");
	poly_print(&a);
	printf("r:\n");
	poly_print(&r);
	if (poly_equal(&a, &r) == 1)
	{
		printf("successfull\n");
	}
	free(CKi);
	free(comList);
}