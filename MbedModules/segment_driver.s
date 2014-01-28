    AREA segment_driver, CODE, READONLY
;--------------------OVERVIEW------------------------
; This file contains ARM Assembly language functions to
; drive a 7-segment LED Display (Kingbright SA03-11HDB).
; This library uses a switch statement on the input
; register (R0) to set or clear each corresponding lead
; of the display.  NOTE: The 7 Segments of the LED display
; must be connected to pins p11 - p17 of the mbed digital
; I/O.  For further help with understanding this file,
; please refer to files:
;                       dig_in.s
;                       dig_out.s
;------------------------------------------------------
; Import function locations so that C compiler can find it and link
    IMPORT dig_out_P11
    IMPORT P11_write
    IMPORT dig_out_P12
    IMPORT P12_write
    IMPORT dig_out_P13
    IMPORT P13_write
    IMPORT dig_out_P14
    IMPORT P14_write
    IMPORT dig_out_P15
    IMPORT P15_write
    IMPORT dig_out_P16
    IMPORT P16_write
    IMPORT dig_out_P17
    IMPORT P17_write
    IMPORT LED1_write
    IMPORT dig_out_LED1    
    IMPORT LED2_write
    IMPORT dig_out_LED2    
    IMPORT LED3_write
    IMPORT dig_out_LED3
    IMPORT LED4_write
    IMPORT dig_out_LED4
; Export function location so that C compiler can find it and link
    EXPORT seg_driver_initialize
seg_driver_initialize       ;CLEAR DISPLAY
    PUSH    {LR}
    BL      dig_out_P11
    BL      dig_out_P12
    BL      dig_out_P13
    BL      dig_out_P14
    BL      dig_out_P15
    BL      dig_out_P16
    BL      dig_out_P17
    BL      dig_out_LED1
    BL      dig_out_LED2
    BL      dig_out_LED3
    BL      dig_out_LED4
    MOV     R0,#1
    BL      P11_write
    BL      P12_write
    BL      P13_write
    BL      P14_write
    BL      P15_write
    BL      P16_write
    BL      P17_write
    BL      LED1_write
    BL      LED2_write
    BL      LED3_write
    BL      LED4_write    
    POP     {LR}
    BX      LR
    
; Export function location so that C compiler can find it and link    
    EXPORT seg_driver
seg_driver                  ;SWITCH STATEMENT
    PUSH    {LR}
    ADR     R2,switchpool
    LDR     PC,[R2,R0,LSL #2]
    ALIGN
switchpool
    DCD case0
    DCD case1
    DCD case2
    DCD case3
    DCD case4
    DCD case5
    DCD case6
    DCD case7
    DCD case8
    DCD case9
case0
    MOV     R0,#0
    BL      P11_write
    BL      P12_write
    BL      P13_write
    BL      P14_write
    BL      P15_write
    BL      P16_write
    BL      LED1_write
    BL      LED2_write
    BL      LED3_write
    BL      LED4_write
    MOV     R0,#1
    BL      P17_write 
    B       end
case1
    MOV     R0,#0
    BL      P12_write
    BL      P13_write
    BL      LED2_write
    BL      LED3_write
    BL      LED4_write        
    MOV     R0,#1
    BL      LED1_write     
    BL      P11_write
    BL      P14_write
    BL      P15_write
    BL      P16_write
    BL      P17_write
    BL      end
case2
    MOV     R0,#0
    BL      P11_write
    BL      P12_write
    BL      P14_write
    BL      P15_write
    BL      P17_write
    BL      LED1_write
    BL      LED3_write
    BL      LED4_write
    MOV     R0,#1
    BL      P13_write
    BL      P16_write
    BL      LED2_write      
    BL      end
case3
    MOV     R0,#0
    BL      P11_write
    BL      P12_write
    BL      P13_write
    BL      P14_write   
    BL      P17_write
    BL      LED3_write    
    BL      LED4_write
    MOV     R0,#1
    BL      LED1_write
    BL      LED2_write    
    BL      P15_write
    BL      P16_write    
    B       end
case4
    MOV     R0,#0    
    BL      P12_write
    BL      P13_write
    BL      P16_write  
    BL      P17_write
    BL      LED1_write
    BL      LED2_write
    BL      LED4_write
    MOV     R0,#1
    BL      P11_write
    BL      P14_write
    BL      P15_write
    BL      LED3_write            
    B       end
case5
    MOV     R0,#0
    BL      P11_write
    BL      P13_write
    BL      P14_write 
    BL      P16_write   
    BL      P17_write
    BL      LED2_write
    BL      LED4_write    
    MOV     R0,#1
    BL      P15_write
    BL      P12_write
    BL      LED1_write
    BL      LED3_write    
    B       end
case6
    MOV     R0,#0
    BL      P11_write       
    BL      P13_write
    BL      P14_write
    BL      P15_write
    BL      P16_write  
    BL      P17_write
    BL      LED1_write
    BL      LED4_write  
    MOV     R0,#1
    BL      P12_write 
    BL      LED2_write
    BL      LED3_write              
    B       end
case7
    MOV     R0,#0
    BL      P11_write
    BL      P12_write       
    BL      P13_write
    BL      LED4_write      
    MOV     R0,#1
    BL      P14_write  
    BL      P15_write
    BL      P16_write
    BL      P17_write
    BL      LED1_write
    BL      LED2_write 
    BL      LED3_write                
    B       end
case8
    MOV     R0,#0
    BL      P11_write
    BL      P12_write       
    BL      P13_write  
    BL      P14_write  
    BL      P15_write
    BL      P16_write
    BL      P17_write
    BL      LED1_write
    BL      LED2_write
    BL      LED3_write         
    MOV     R0, #1
    BL      LED4_write           
    B       end
case9
    MOV     R0,#0
    BL      P11_write
    BL      P12_write
    BL      P13_write
    BL      P14_write   
    BL      P17_write
    BL      P16_write
    BL      LED2_write
    BL      LED3_write      
    MOV     R0,#1
    BL      P15_write
    BL      LED1_write
    BL      LED4_write     
    B       end
end
    POP     {LR}
    BX      LR
;
    END
    