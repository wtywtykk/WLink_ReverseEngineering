void app_many_swd_op_9()
{
	memset(app_swd_res_buf_d0,0,7);
	
	if(USB_DATA_FLAGS2.4)
	{
		//ROM_CD1
		R5=app_swd_direct_buf_op;
		R4=0
		if(app_swd_direct_buf_op==1)
			app_swd_direct_buf_adr=app_swd_direct_buf_adr<<1;
		else if(app_swd_direct_buf_op==2)
			app_swd_direct_buf_adr=app_swd_direct_buf_adr<<1 | 1;
		RAM_1D5=app_swd_direct_buf_adr;
		RAM_1D6=0;
		
		app_swd_cnt=0;
		do(app_swd_cnt<8)
		{
			RAM_1D6^= app_swd_direct_buf_adr;
			app_swd_direct_buf_adr>>=1;
			app_swd_cnt++;
		}
		
		P1_MOD_OC |=0x10
		P1_DIR_PU |=0x10
		P1_MOD_OC &=0xFD
		P1_DIR_PU |=0x2
		
		if(XRAM_1D5 & 1)
		{
			//ROM_D48
			//8 nops per toggle
			RAM_1D1=RAM_1CA;
			RAM_1D2=RAM_1CB;
			RAM_1D3=RAM_1CC;
			RAM_1D4=RAM_1CD;
			
			RAM_5DA=RAM_1D1;
			RAM_5DB=RAM_1D2;
			RAM_5DC=RAM_1D3;
			RAM_5DD=RAM_1D4;
			
			app_swd_direct_buf_op=0;
			
			app_swd_cnt=0;
			do(app_swd_cnt<8)
			{
				app_swd_direct_buf_op^= RAM_1CA;
				RAM_1CA>>=1;
				app_swd_cnt++;
			}
			
			app_swd_cnt=0;
			do(app_swd_cnt<8)
			{
				app_swd_direct_buf_op^= RAM_1CB;
				RAM_1CB>>=1;
				app_swd_cnt++;
			}
			
			app_swd_cnt=0;
			do(app_swd_cnt<8)
			{
				app_swd_direct_buf_op^= RAM_1CC;
				RAM_1CC>>=1;
				app_swd_cnt++;
			}
			
			app_swd_cnt=0;
			do(app_swd_cnt<8)
			{
				app_swd_direct_buf_op^= RAM_1CD;
				RAM_1CD>>=1;
				app_swd_cnt++;
			}

			XRAM_1D6 &= 1;
			if(app_swd_direct_buf_op & 1)
				XRAM_1D6 |=2;
			P1.1=1;
			P1.4=1;
			P1.1=0;
			P1.4=0;
			
			//ROM_E71
			app_swd_cnt=0;
			do{
				app_swd_cnt++;
				
				if(XRAM_1D5 & 0x80)
					P1.1=1
				else
					P1.1=0
				P1.4=1
				XRAM_1D5 <<=1;
				P1.4=0
			}while(app_swd_cnt<8);
			if(XRAM_1D6 & 1)
				P1.1=1
			else
				P1.1=0
			P1.4=1
			P1.4=0
			P1_MOD_OC &=0xFD
			P1_DIR_PU &=0xFD
			P1.4=1
			P1.4=0
			P1.1=0
			P1.4=1
			swd_some_rd_result=P1.1;
			if(swd_some_rd_result)
				app_swd_res_buf_op=1;
			else
				app_swd_res_buf_op=0;
			P1.4=0
			P1_MOD_OC &=0xFD
			P1_DIR_PU &=0xFD
			P1.4=1
			P1.4=0
			P1.1=0
			P1_MOD_OC &=0xFD
			P1_DIR_PU |=0x2
			P1.4=1
			P1.4=0
			P1_MOD_OC &=0xFD
			P1_DIR_PU &=0xFD
			P1.4=1
			P1.4=0
			P1_MOD_OC &=0xFD
			P1_DIR_PU |=0x2
			
			//ROM_F67
			app_swd_cnt=0;
			do{
				app_swd_cnt++;
				
				if(XRAM_1D4 & 0x80)
					P1.1=1
				else
					P1.1=0
				P1.4=1
				XRAM_1D4 <<=1;
				P1.4=0
			}while(app_swd_cnt<8);
			app_swd_cnt=0;
			do{
				app_swd_cnt++;
				
				if(XRAM_1D3 & 0x80)
					P1.1=1
				else
					P1.1=0
				P1.4=1
				XRAM_1D3 <<=1;
				P1.4=0
			}while(app_swd_cnt<8);
			app_swd_cnt=0;
			do{
				app_swd_cnt++;
				
				if(XRAM_1D2 & 0x80)
					P1.1=1
				else
					P1.1=0
				P1.4=1
				XRAM_1D2 <<=1;
				P1.4=0
			}while(app_swd_cnt<8);
			app_swd_cnt=0;
			do{
				app_swd_cnt++;
				
				if(XRAM_1D1 & 0x80)
					P1.1=1
				else
					P1.1=0
				P1.4=1
				XRAM_1D1 <<=1;
				P1.4=0
			}while(app_swd_cnt<8);
			if(XRAM_1D6 & 2)
				P1.1=1
			else
				P1.1=0
			P1.4=1
			P1.4=0
			P1_MOD_OC &=0xFD
			P1_DIR_PU &=0xFD
			P1.4=1
			P1.4=0
			P1.1=0
			P1.4=1
			swd_some_rd_result=P1.1;
			if(swd_some_rd_result)
				app_swd_res_buf_op=1;
			else
				app_swd_res_buf_op=0;
			P1.4=0
			//ROM_10A1 GOTO ROM_12F8
		}
		else
		{
			//ROM_10A4
			//8 nops per toggle
			P1.4=1
			P1.1=1
			P1.1=0
			P1.4=0
			app_swd_cnt=0;
			do{
				app_swd_cnt++;
				
				if(XRAM_1D5 & 0x80)
					P1.1=1
				else
					P1.1=0
				P1.4=1
				XRAM_1D5 <<=1;
				P1.4=0
			}while(app_swd_cnt<8);
			if(XRAM_1D6 & 1)
				P1.1=1
			else
				P1.1=0
			P1.4=1
			P1.4=0
			P1_MOD_OC &=0xFD
			P1_DIR_PU &=0xFD
			P1.4=1
			P1.4=0
			P1.1=0
			P1.4=1
			swd_some_rd_result=P1.1;
			if(swd_some_rd_result)
				app_swd_res_buf_op=1;
			else
				app_swd_res_buf_op=0;
			P1.4=0
			P1_MOD_OC &=0xFD
			P1_DIR_PU &=0xFD
			P1.4=1
			P1.4=0
			P1.1=0
			P1_MOD_OC &=0xFD
			P1_DIR_PU |=0x2
			P1.4=1
			P1.4=0
			P1_MOD_OC &=0xFD
			P1_DIR_PU &=0xFD
			P1.4=1
			P1.4=0
			
			//ROM_11B0
			app_swd_cnt=0;
			do{
				app_swd_cnt++;
				P1.4=1;
				app_swd_res_buf_d3 <<=1;
				if(P1.1)
					app_swd_res_buf_d3|=1;
				P1.4=0;
			}while(app_swd_cnt<8);
			app_swd_cnt=0;
			do{
				app_swd_cnt++;
				P1.4=1;
				app_swd_res_buf_d2 <<=1;
				if(P1.1)
					app_swd_res_buf_d2|=1;
				P1.4=0;
			}while(app_swd_cnt<8);
			app_swd_cnt=0;
			do{
				app_swd_cnt++;
				P1.4=1;
				app_swd_res_buf_d1 <<=1;
				if(P1.1)
					app_swd_res_buf_d1|=1;
				P1.4=0;
			}while(app_swd_cnt<8);
			app_swd_cnt=0;
			do{
				app_swd_cnt++;
				P1.4=1;
				app_swd_res_buf_d0 <<=1;
				if(P1.1)
					app_swd_res_buf_d0|=1;
				P1.4=0;
			}while(app_swd_cnt<8);

			P1.4=1;
			swd_some_rd_result=P1.1;
			if(swd_some_rd_result)
				app_swd_res_buf_op=1;
			P1.4=0
			P1_MOD_OC &=0xFD
			P1_DIR_PU &=0xFD
			P1.4=1
			P1.4=0
			P1.1=0
			P1.4=1
			P1.4=1;
			swd_some_rd_result=P1.1;
			if(swd_some_rd_result)
				app_swd_res_buf_op=1;
			else
				app_swd_res_buf_op=0;
			P1.4=0
			P1_MOD_OC &=0xFD
			P1_DIR_PU &=0xFD
			P1.4=1
			P1.4=0
			P1.1=1
			P1_MOD_OC &=0xFD
			P1_DIR_PU |=0x2
			P1.4=1
			P1.4=0
			P1_MOD_OC &=0xFD
			P1_DIR_PU &=0xFD
			P1.4=1
			P1.4=0
			P1_MOD_OC &=0xFD
			P1_DIR_PU |=0x2
			P1.1=0;
			P1.4=1;
			P1.1=1;
			P1_MOD_OC |=0x10
			P1_DIR_PU |=0x10
			
			//ROM_1382 GOTO ROM_12F8
		}
	}
	else
	{
		//ROM_1385
		P1_MOD_OC &=0xFD
		P1_DIR_PU |=2
		P1_MOD_OC &=0xEF
		P1_DIR_PU |=0x10
		
		if(USB_DATA_FLAGS3.0)//LowSpeed?
		{
			//ROM_1397
			//20 nops per toggle
			P1.4=1
			P1.1=1
			nop;
			P1.1=0
			nop;
			P1.4=0
			nop;
			
			app_swd_direct_buf_adr<<=1;
			app_swd_cnt=0;
			
			do{
				app_swd_cnt++;
				
				if(app_swd_direct_buf_adr & 0x80)
					P1.1=1
				else
					P1.1=0
				nop;
				P1.4=1
				nop;
				app_swd_direct_buf_adr <<=1;
				P1.4=0
			}while(app_swd_cnt<7);
			app_swd_cnt=0;
			do{
				app_swd_cnt++;
				
				if(app_swd_direct_buf_d3 & 0x80)
					P1.1=1
				else
					P1.1=0
				nop;
				P1.4=1
				nop;
				app_swd_direct_buf_d3 <<=1;
				P1.4=0
			}while(app_swd_cnt<8);
			app_swd_cnt=0;
			do{
				app_swd_cnt++;
				
				if(app_swd_direct_buf_d2 & 0x80)
					P1.1=1
				else
					P1.1=0
				nop;
				P1.4=1
				nop;
				app_swd_direct_buf_d2 <<=1;
				P1.4=0
			}while(app_swd_cnt<8);
			app_swd_cnt=0;
			do{
				app_swd_cnt++;
				
				if(app_swd_direct_buf_d1 & 0x80)
					P1.1=1
				else
					P1.1=0
				nop;
				P1.4=1
				nop;
				app_swd_direct_buf_d1 <<=1;
				P1.4=0
			}while(app_swd_cnt<8);
			app_swd_cnt=0;
			do{
				app_swd_cnt++;
				
				if(app_swd_direct_buf_d0 & 0x80)
					P1.1=1
				else
					P1.1=0
				nop;
				P1.4=1
				nop;
				app_swd_direct_buf_d0 <<=1;
				P1.4=0
			}while(app_swd_cnt<8);
			
			//ROM_15D2 some bit shifting
			app_swd_direct_buf_op=(app_swd_direct_buf_op & 0x3) << 6
			if(app_swd_direct_buf_op & 0x80)
				P1.1=1;
			else
				P1.1=0;
				
			//1C25
			nop;
			P1.4=1;
			nop;
			app_swd_direct_buf_op<<=1;
			P1.4=0;
			if(app_swd_direct_buf_op & 0x80)
				P1.1=1;
			else
				P1.1=0;
				
			//1C4F
			nop;
			P1.4=1;
			nop;
			app_swd_direct_buf_op<<=1;
			nop;
			P1.4=0;
			P1_MOD_OC &=0xFD
			P1_DIR_PU &=0xFD
			nop;
			
			A=P1.1
			swd_some_rd_result=A;
			swd_rd_tmp=A & 0x1;
			P1.4=1;
			app_swd_res_buf_nk+=swd_rd_tmp;
			nop;
			P1.4=0;
			nop;
			app_swd_cnt=0;
			
			do{
				app_swd_cnt++;
				nop;
				A=P1.1
				swd_some_rd_result=A;
				swd_rd_tmp=A & 0x1;
				P1.4=1;
				app_swd_res_buf_adr<<=1;
				app_swd_res_buf_adr|=swd_rd_tmp;
				nop;
				P1.4=0;
			}while(app_swd_cnt<7);
			
			app_swd_cnt=0;
			do{
				app_swd_cnt++;
				A=P1.1
				swd_some_rd_result=A;
				swd_rd_tmp=A & 0x1;
				P1.4=1;
				app_swd_res_buf_d3<<=1;
				app_swd_res_buf_d3|=swd_rd_tmp;
				P1.4=0;
			}while(app_swd_cnt<8);
			
			app_swd_cnt=0;
			do{
				app_swd_cnt++;
				A=P1.1
				swd_some_rd_result=A;
				swd_rd_tmp=A & 0x1;
				P1.4=1;
				app_swd_res_buf_d2<<=1;
				app_swd_res_buf_d2|=swd_rd_tmp;
				P1.4=0;
			}while(app_swd_cnt<8);
			
			app_swd_cnt=0;
			do{
				app_swd_cnt++;
				A=P1.1
				swd_some_rd_result=A;
				swd_rd_tmp=A & 0x1;
				P1.4=1;
				app_swd_res_buf_d1<<=1;
				app_swd_res_buf_d1|=swd_rd_tmp;
				P1.4=0;
			}while(app_swd_cnt<8);
			
			app_swd_cnt=0;
			do{
				app_swd_cnt++;
				A=P1.1
				swd_some_rd_result=A;
				swd_rd_tmp=A & 0x1;
				P1.4=1;
				app_swd_res_buf_d0<<=1;
				app_swd_res_buf_d0|=swd_rd_tmp;
				P1.4=0;
			}while(app_swd_cnt<8);
			
			nop;
			A=P1.1
			swd_some_rd_result=A;
			swd_rd_tmp=A & 0x1;
			P1.4=1;
			app_swd_res_buf_op<<=1;
			app_swd_res_buf_op+=swd_rd_tmp;
			nop;
			P1.4=0;
			nop;
			
			A=P1.1
			swd_some_rd_result=A;
			swd_rd_tmp=A & 0x1;
			P1.4=1;
			app_swd_res_buf_op<<=1;
			app_swd_res_buf_op+=swd_rd_tmp;
			nop;
			P1.4=0;

			P1_MOD_OC &=0xFD
			P1_DIR_PU |=0x2
			nop;
			P1.4=1;
			nop;

			if(app_swd_direct_buf_fin)
				P1.1=1;
			else
				P1.1=0;
			P1.4=0;
			nop;
			P1.1=0;
			nop;
			P1.4=1;
			nop;
			P1.1=1;
			
			//ROM_1BAF JMP TP ROM_1F15
		}
		else
		{
			//faster version
			//13 nops per toggle
		}
		//ROM_1F15
		P1_MOD_OC |=0x10
		P1_DIR_PU |=0x10
	}
	
	//ROM_1F28
	P1_MOD_OC &=0xFD
	P1_DIR_PU &=0xFD
	
	R3=1;
	R2=5;
	R1=0xDA;
}
