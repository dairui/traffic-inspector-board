/**
  @page IAP over Ethernet Readme file
 
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    IAP/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    07/16/2010
  * @brief   Description of the ANxxxx "STM32F107 in-application programming
  *          over Ethernet (IAP over Ethernet)".
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files that implement In-Application 
Programming (IAP) over Ethernet. Two IAP options are provided: 
 - IAP using TFTP (Trivial File Transfer Protocol)
 - IAP using HTTP (Hypertext Transfert Protocol) 
User has choice through compile options (in main.h file) to include one or both 
options.
Please note that for both IAP options, LwIP v1.3.1 is used as the TCP/IP stack. 


@par Project Directory contents 

 - "inc": contains the IAP firmware header files 
    - inc/main.h             main config file          
    - inc/flash_if.h         header for flash_if.c
    - inc/httpserver.h       header for httpserver.c
    - inc/tftpserver.h       header for tftpserver.c
    - inc/stm32f107.h        header for stm32f107.c
    - inc/netconf.h          header for netconf.c
    - inc/lwipopts.h         LwIP stack configuration options
    - inc/fsdata.h           header for fsdata.c  
    - inc/stm32f10x_conf.h   Library Configuration file
    - inc/stm32f10x_it.h     Header for stm32f10x_it.c    

 - "EWARMv5": contains pre-configured project for EWARM5 toolchain
 - "RVMDK": contains pre-configured project for RVMDK toolchain 

 - "src": contains the IAP firmware source files
    - src/main.c             main program file          
    - src/flash_if.c         high level functions for flash access (erase, write)
    - src/httpserver.c       IAP http server functions
    - src/tftpserver.c       IAP tftpserver functions
    - src/stm32f107.c        STM32F107 BSP functions (system configurations)
    - src/netconf.c          LwIP stack initializations
    - src/fsdata.c           ROM filesystem data (html pages) 
    - src/stm32f10x_it.c     STM32 Interrupt handlers   

@par Hardware and Software environment  
  
  - This example has been tested with STMicroelectronics STM3210C-EVAL (STM32F10x 
    Connectivity line).

  - STM3210C-EVAL Set-up 
    - Connect STM3210C-EVAL board to remote PC (through a crossover ethernet cable)
      or to your local network (through a straight ethernet cable)
    - Use the Key push-button (connected to pin PB.09)
    - STM3210C-EVAL jumpers configuration
       ==============================================================
        Jumper  |  MII mode configuration  |  RMII mode configuration
       ==============================================================
        JP2     |      Not connected       |     Connected
        JP3     |          2-3             |       1-2
        JP4     |          1-2             |       2-3
        -------------------------------------------------------------
        JP11    |                     2-3
        JP12    |                     2-3
        JP13    |                     2-3
        JP14    |                     1-2
     
                    
  - PC utility TFTPD32 (http://tftpd32.jounin.net/) is used as a DHCP server and a TFTP client
  - Firefox Mozilla (v3.6 and later) or Microsoft Internet Explorer (v8 and later) 
    are used as internet browsers (Http clients)
  @Note: When connecting the board to ST network, you have to disable the proxy (if any)


@par How to use it ? 

In order to make the program work, you must do the following:
1. in main.h file Select the IAP compile options
2. Build and Program IAP code into STM32F107 flash
3. In order to enter in IAP mode, Reset the board while keeping Key button pressed
4. When using DHCP or TFTP (see main.h IAP options), user should launch TFTPD32 
   utility on test PC (be sure of TFTP client and DHCP setting options).
5. If DHCP option is enabled in IAP code, then a DHCP session starts, 
   otherwise the static IP address (192.168.0.10) is used
6. After enf of IP address configuration, user can start In Application programming
   either through a TFTP client (TFTPD32) or a web browser (Firefox Mozilla or MSIE).
7. In case of IAP using TFTP, in TFPD32 select TFTP client panel, browse for the
   binary image to be loaded into STM32 Flash, then press Put button. At the end
   of file transfer reset the board to run new application
8. In case of IAP using HTTP, open a web browser and type the board IP address,
   a simple file-upload Html form will be shown on browser, select the binary
   file then press Upload button. At the end of transfer, a new html page is loaded
   indicating transfer success with a press button to Reset MCU. 
      
      
In order to load the IAP code, you have do the following:

 - EWARMv5 (v5.50 and later):
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 - ARM-MDK(v4.11 and later):
    - Open the Project.Uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)
    
 - RIDE(v7):
    - Open the Project.rprj project
    - In the configuration toolbar(Project->properties) select the project config:
        - STM3210C-EVAL: to configure the project for STM32 Connectivity line devices
    - Rebuild all files: Project->build project
    - Load project image: Debug->start(ctrl+D)
    - Run program: Debug->Run(ctrl+F9)

 - HiTOP(v5.32 and later):
    - Open the HiTOP toolchain.
    - Browse to open the Project.htp
    - A "Download application" window is displayed, click "cancel".
    - Rebuild all files: Project->Rebuild all
    - Load project image : Click "ok" in the "Download application" window.
    - Run the "RESET_GO_MAIN" script to set the PC at the "main"
    - Run program: Debug->Go(F5).  
    
 - TrueSTUDIO(v1.4.0 and later):
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace 
      directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory and select the project: 
        - STM3210C-EVAL: to load the project for Connectivity line devices
    - Under Windows->Preferences->General->Workspace->Linked Resources, add 
      a variable path named "CurPath" which points to the folder containing
      "Libraries", "Project" and "Utilities" folders.
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)
 
 * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
 */
