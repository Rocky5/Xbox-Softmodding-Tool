/*
**********************************
**********************************
**      BROUGHT TO YOU BY:		**
**********************************
**********************************
**								**
**		  [TEAM ASSEMBLY]		**
**								**
**		www.team-assembly.com	**
**								**
******************************************************************************************************
* This is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
******************************************************************************************************
*
* Source modified by Rocky5.
* Used with the 2014/16 Softmod kit.
*
******************************************************************************************************
*/
#include "Xbox Softmod Tool Kit.h"
#include "ShadowC Header.h"
#include <d3d8.h>
#include "xkhdd.h"
#include "xkeeprom.h"
#include <fstream>
#include <iostream>
#include <time.h>
#include <cstdio>
#include <sys/stat.h>
extern "C" XPP_DEVICE_TYPE XDEVICE_TYPE_IR_REMOTE_TABLE;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shared paths
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PrepDir												"E:\\Prep\\"
#define NKPSDir												"E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\"
#define NKPDir												"C:\\NKPatcher\\"
#define NKPConfDir											"C:\\NKPatcher\\Configs\\"
#define NKPDirAlt											"S:\\NKPatcher\\"
#define NKPConfDirAlt										"S:\\NKPatcher\\Configs\\"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Save path for the EEPROM files
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Backup_Path											"E:\\Backups\\EEPROM\\"
#define Virtual_Full_Path				NKPConfDirAlt		"eeprom.bin"
//#define Backup_Path "D:\\"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create Directory paths
// Cant seem to create a dir structure with one path, so had to use two
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Backup_EEPROM_Save_Path1							"E:\\Backups"
#define Backup_EEPROM_Save_Path2							"E:\\Backups\\EEPROM"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Config files, or if you will enabler files
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Virtual_Remove_File									"D:\\Virtual_EEPROM_Remove.bin"
#define Virtual_File										"D:\\Virtual_EEPROM_Backup.bin"
#define Reboot_File											"D:\\Reboot.bin"
#define Override_File										"D:\\Standalone_Mode.bin"
#define RNKP_File											"D:\\rnkp.bin"
#define Update_Font											"D:\\Update_Font.bin"
#define Restore_Font										"D:\\Restore_Font.bin"
#define NULLHDDKey_File										"D:\\NULL_HDD_Key.bin"
#define PatchNKP_File1										"D:\\NKP_Part.bin"
#define PatchNKP_File2										"D:\\NKP_Disable_DB.bin"
#define PatchNKP_File3										"D:\\NKP_DB.bin"
#define Region_File1										"D:\\NTSC.bin"
#define Region_File2										"D:\\PAL.bin"
#define NullChecksum3_File									"D:\\NullChecksum3.bin"
#define FactoryResetPAL_File								"D:\\FactoryResetPAL.bin"
#define FactoryResetNTSC_File								"D:\\FactoryResetNTSC.bin"
#define EnableControlledError16_File						"D:\\EnablePersistentSoftmodMode.bin"
#define DisableControlledError16_File						"D:\\DisablePersistentSoftmodMode.bin"
#define Change_Language_File								"D:\\Change_Language.bin"
#define English_File										"D:\\English.bin"
#define Japanese_File										"D:\\Japanese.bin"
#define German_File											"D:\\German.bin"
#define French_File											"D:\\French.bin"
#define Spanish_File										"D:\\Spanish.bin"
#define Italian_File										"D:\\Italian.bin"
#define Korean_File											"D:\\Korean.bin"
#define Chinese_File										"D:\\Chinese.bin"
#define Portuguese_File										"D:\\Portuguese.bin"
#define DecryptedEERPOM_File								"D:\\BackupDEEPROM"
#define Backup_Bios											"D:\\Backup_Bios.bin"
#define Remove_Parental_Controls							"D:\\Remove_PC.bin"
#define LockHDD_File					PrepDir				"LockHDD.xbe"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dashboard Files
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RunDashboard										"C:\\Dashboard\\default.xbe"
#define DashboardXBE					NKPSDir				"configs\\alt xbe's\\dashboard.xbe"
#define DashboardXML					NKPSDir				"configs\\dash.xml"
#define DashboardSkinXML				NKPSDir				"Skins\\Dashboard\\skin.xml"
#define DashboardSkinwideXML			NKPSDir				"Skins\\Dashboard\\skinwide.xml"
#define DashboardSkinevoxXML			NKPSDir				"Skins\\Dashboard\\skinevox.xml"
#define DashboardSkinSplash				NKPSDir				"Skins\\Dashboard\\UXSplash.jpg"
#define UnleashXXBE						NKPSDir				"Unleashx.xbe"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enable/Disable files
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EnabledPNG						NKPSDir				"toggles\\enabled.png"
#define Dummy_File						NKPSDir				"modules\\Xbox softmod tool kit\\dummy_file"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup Kernel selection for first run of softmod
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NKP_XBE							NKPDirAlt			"default.xbe"
#define Dashloader											"S:\\nkpatcher\\dashloader\\default.xbe"
#define Rescuedashloader									"S:\\nkpatcher\\rescuedash\\loader.xbe"
#define PrepUX							PrepDir				"unleashx.xbe"
#define PrepXBE							PrepDir				"default.xbe"
#define SecondRun											"D:\\secondrun.bin"
#define SecondRunUpdate										"D:\\secondrunupdate.bin"
#define FirstRunBin											"D:\\firstrun.bin"
#define UpdateRunBin										"D:\\updaterun.bin"
#define Font_Path						NKPConfDir			"fonts\\"
#define Font_Path_Alt 					NKPConfDirAlt		"fonts\\"
#define Generic_Font_File				NKPConfDir			"Fonts\\generic.xtf"
#define Generic_Font_File_Alt			NKPConfDirAlt		"Fonts\\generic.xtf"
#define xbox_xtf_File										"C:\\xodash\\xbox.xtf"
#define xbox_xtf_File_Alt									"S:\\xodash\\xbox.xtf"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup ShadowC selection for first run of softmod
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ShadowC_Location				NKPDir				"shadowc\\shadowc.img"
#define ShadowC_Size										485
#define ShadowC_Size_Alt									480
#define ShadowCOFF						NKPConfDir			"shadowc_off.bin"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup UnleashX for showing the ShadowC partition
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NKPatcherSettings				NKPSDir				"UnleashX.xbe"
#define ResoftmodDash					NKPSDir				"modules\\resoftmod dash\\default.xbe"
#define CRescueDash						NKPDir				"rescuedash\\unleashx.xbe"
#define DashSettings					NKPSDir				"modules\\dash settings\\default.xbe"
#define ERescueDash1										"E:\\TDATA\\Rescuedash\\default.xbe"
#define ERescueDash2										"E:\\UDATA\\Rescuedash\\default.xbe"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup modifications for the xbe files
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NKPatcher default
#define NKPHabibi_F_No_DB					"\x58\x42\x45\x48\x8D\x63\xA1\xB7\xDC\xC0\xEF\x5D\x6C\xC4\x91\x99\xAC\xF6\xFB\x3F\x5A\x64\xD0\x69\xE9\xD0\x5B\x7F\xAA\xB2\x09\x72\x3A\x12\x91\x92\xFC\x61\x94\x79\x56\xA4\x65\xB9\x46\x07\xCF\x66\x2D\xD1\x39\xA4\x6D\x38\xB9\x93\x27\x22\x78\x96\x21\x48\x6B\x57\xA2\xEE\x1D\xFB\xD6\x2A\xA1\xC8\x42\x77\x6B\xA2\xDF\xD1\xAF\x0C\x57\xB9\x0C\xE5\xCE\x44\xBA\x80\x30\xE3\x42\x24\xD6\x9E\x4E\xB6\x64\x82\x54\xA2\x53\x38\xE6\x98\xF8\x6E\xC8\xE2\xD9\x1A\xC2\xDE\xB9\x25\xD4\x41\x61\x15\x31\xA9\x94\xA6\x6F\x19\x1D\xB0\xB3\x71\x34\x48\x7E\x30\xF8\x11\x2A\x6D\xE6\x58\xB0\xE1\x3C\xB8\xE0\xA0\x32\x4C\xFE\xDE\xE4\x51\x66\x17\x18\xF2\xB3\x55\x09\xE5\x60\xDE\x6F\xA1\x13\x88\x62\xE4\x1D\xBD\x9B\x44\x05\x2F\xE8\x77\x5A\x32\x8C\xCA\xE6\x69\x69\x2C\x8C\xE5\x97\x29\xCC\x19\x28\x25\x93\xA1\x6C\x13\x0C\xCE\x53\xDA\x54\xE9\xE6\x1D\xBB\x1E\x90\x66\x75\xE2\x40\xE1\x5A\x24\x63\x46\x93\xC8\x22\x73\x18\xAC\x2A\x51\x65\xD1\xF6\x73\x95\xB0\xD1\x10\xD2\x00\xC9\x52\xC9\x47\x4F\x00\x4E\x8D\x91\x3B\x46\x1F\xEA\xAB\x55\x80\x86\x16\x40\x0A\x1F\x1E\xF7\x55\xD4\x94\x1B\x14"
#define NKPHash_F_No_DB						"\x32\xD1\x88\x43\xDF\xC2\x62\x8F\xB0\xDD\x8F\x5D\xAA\x44\x47\x33\x2A\xA1\x37\x18"
//
#define NKPHabibi_F_G_No_DB					"\x58\x42\x45\x48\x4A\x5E\xF9\x92\x98\x49\x1A\xB7\x79\xBE\x39\x81\xD4\x82\x14\x9E\x51\x6A\xB5\xFE\x4B\xE5\x90\xE7\x0E\x53\x86\x40\xC4\xD4\xDA\x56\x70\xF4\xB5\xBA\x69\xEE\x48\x9B\x38\xB7\xB2\x64\xB3\x38\xDE\xD0\x10\x45\x38\xE2\x5D\xDE\x86\x41\x63\x20\x1C\x8F\x2C\x92\xB7\x2E\xED\x9A\xD4\x50\x37\xAE\xC4\xE0\x77\x83\xCE\xB7\x9C\xD6\x52\x99\xD0\x28\xCE\xE0\x27\x95\xE5\x02\x65\xC0\x42\x93\x4D\x37\xD8\xC4\xCC\x88\xBC\xC6\x42\x60\xEB\x4A\xEE\x40\xC5\x6B\xC8\x8C\x7A\xB2\xE9\x26\xAF\x06\x0E\x52\xF8\x21\xB3\xB9\xEC\x49\x53\x71\x35\x98\x16\x5E\x5B\x4D\x06\x87\x3B\x31\xA2\x28\x1F\xF1\x68\xBD\x93\xBA\x11\xC2\x65\xED\x65\x20\x25\x06\xB4\xB0\x3B\xFF\xA2\x34\x43\x5D\x13\x4E\x70\xE5\x65\xEC\xE2\x89\x1C\x61\x87\x13\x9B\x04\x7C\xDE\x7B\x00\x0A\xF7\x9A\x61\xC8\x24\x1D\xFB\x36\x30\x85\xCC\x88\xF2\x00\x4C\xC6\x97\x2A\x9D\x12\xD0\x97\xD5\xCC\x3E\xC8\xC3\x9B\x83\x60\x9F\x0B\xF6\x3C\x1F\x6B\x17\xBC\xEC\x5F\x42\xE4\xFA\x28\xED\x43\x5F\x57\x73\xA1\x2B\x5F\x94\x3E\x9B\x9A\x43\x41\x7A\x5F\xC9\x05\xD3\xCF\xC9\x0B\x73\xBE\x8D\x10\x26\xF2\x9C\xDE\xFB\xD9\x88"
#define NKPHash_F_G_No_DB					"\x97\x50\x97\xC3\x74\x5A\x44\x09\x08\x33\xFD\x04\xC4\xD2\xF4\xDF\x11\x23\x51\xA4"
//
#define NKPHabibi_F_DB						"\x58\x42\x45\x48\xB2\x3E\x1C\xA5\xED\x33\x2A\xE2\x15\x67\x70\x29\x3C\xAC\x5E\x98\x4A\x28\x41\xCC\xE1\x0D\x51\xC4\xDF\xC1\xF5\x58\xE0\x57\x1A\xB7\xF8\xDF\x34\x56\xC7\x6D\xC5\x8E\xE6\x27\x3C\xC7\x8C\x2D\xF9\x6F\x54\x31\x08\xA2\x1D\xDE\x99\xD2\xF4\xFA\x0D\x5F\x11\x2E\x2A\x3F\xF2\x1B\x61\x8F\x23\x6D\x69\x19\xCD\xD9\x40\xA6\xD1\xEC\xF4\xFF\x4D\x94\xA1\xD9\x2D\xED\x63\xAD\xF2\xEB\xD2\x88\xFF\x41\x34\x64\xC6\xF7\x99\x0E\xB5\x45\xB5\x7E\xD4\xB2\x13\x34\xAC\x8A\x5D\xD9\x16\x39\xF2\xB3\x69\xCE\xFC\xFC\x5E\xF8\x08\xC8\x2A\xFC\x55\xDF\xAB\x13\x7C\xD4\x88\xC5\xFF\xA7\x17\x24\xBD\x87\x61\x11\xD5\xD8\x2D\xE6\xE5\x35\x90\xEC\x63\xF1\xF5\x89\xF8\x1E\x1E\xFE\x5D\x8D\xE0\xF2\x97\x38\x3D\x19\x6C\x5F\xF2\xE7\x55\x15\x68\x4C\xD6\xD4\xAC\x48\xC0\x50\xC4\xCD\x30\xBB\xD0\xFC\xA2\xFA\xA4\x97\x0C\x46\x5D\x30\x41\x8A\xE0\xB3\x7F\x65\x01\x06\x80\x2C\x6D\x0E\x0A\x91\xBE\x8E\x99\x23\xFD\xF2\xDC\xF7\xC5\xB1\x2A\x6D\x43\x2F\xF5\x96\x37\x60\x26\x36\x77\x25\xFD\x0C\x36\xE3\xEA\xB7\x0B\x9B\xFC\x68\xA6\x7F\xCD\x12\x47\x9B\x4F\xA2\xF4\x2D\x0A\xA0\xA1\x3D\x0A\x58"
#define NKPHash_F_DB						"\x15\xF4\x83\x44\xD9\xE2\xE3\x75\x36\x3F\x42\xBB\xC1\x3E\xED\x67\x1F\x4B\xEE\xAE"
//
#define NKPHabibi_F_G_DB					"\x58\x42\x45\x48\x58\x30\x39\xED\x51\xB7\x64\x07\x8F\xD5\x1E\xE6\xE3\x7E\xA4\x72\xD2\xF5\xF5\x52\xDE\xDB\x1D\x03\xCF\xF9\x49\x85\x5B\xC0\xC3\x4A\x52\x68\x72\xAF\x7D\x04\xDD\x14\xFE\x91\x58\x9A\x8F\xCE\x28\x84\x9F\x9D\x22\xE8\xE3\xD8\xF9\x7B\x9B\x8B\x17\x5F\x6A\x27\xEC\xD5\xE3\x1D\x2A\xD1\x2C\x63\x25\x10\xDE\x1C\xBA\x4F\x5E\x47\x7D\x73\x50\xCA\x8D\x15\x8E\xB5\x73\x37\x81\xBD\x7F\xDB\x44\x0C\xF5\x63\x93\xDD\x6B\xCF\x66\xF1\x6A\xD9\xCD\xED\x43\x17\xE7\x25\x19\x38\xA0\xD1\x71\x6A\x67\xDF\x36\xD6\x33\xC5\x61\x9F\xD1\xDB\x06\x31\xF2\x4E\x5E\x63\x9F\x3C\x10\x5D\xEB\xA7\xBE\x5F\x84\xC0\x9F\xDC\x1B\x7D\x72\xAE\xE2\xBB\x69\x9A\xE6\xFC\xC9\x40\x5D\xC2\xD4\x9A\xDC\x04\x2D\xD3\x9E\x17\x85\xB7\x8A\xC2\x25\xCA\xEB\xD1\x06\xA8\x81\xF6\xB8\xD2\x6E\xB5\x79\x0D\xCF\xA0\xF2\x17\xE1\xDD\x1E\x27\x50\xD6\x25\xCD\xAC\x06\xFD\xC2\xEA\x40\x14\x1E\xC7\xC6\x71\x6B\xF5\x06\x73\x8A\xDD\xB0\x18\x8F\x02\xB5\xD0\x1D\xD4\x87\xA1\xC0\xDC\x86\x60\xA7\x8A\xA2\x07\xDC\xA6\xC0\xB7\xD4\xA9\x52\x22\xF8\x70\x3F\x4A\xD0\xAF\xC0\x51\x21\x90\x93\x69\xBD\x61\x40\xD2\x66"
#define NKPHash_F_G_DB						"\x05\xC1\x2A\xA5\x8F\x35\x9E\x76\x5A\x5C\x77\xF6\x23\x39\x9B\xE6\xB6\x39\xAE\x9F"
//
#define NKPValue1							"\x01"
#define NKPValue2							"\x02"
#define NKPValue3							"\x08"
#define NKPValue4							"\x09"
#define NKPValue1Size						1
// NKPatcher Settings
#define NKPSHabibi							"\x58\x42\x45\x48\x36\x52\x21\x23\xAC\xB1\x16\xDF\xC6\x41\xAC\xD6\xD4\x9C\xDC\x3C\x44\xB2\x5B\xB1\x53\xD5\x2C\xBA\x04\xC6\x01\x5D\x19\x6F\xA1\x7E\x71\x02\xFE\x93\xD7\x3C\xC1\x82\xAF\x9D\x6B\x02\x7A\xC1\x61\x3E\x41\xB2\x90\xB7\x11\x8D\x1D\xAA\x9B\x63\x83\xF4\xE7\x7B\x2C\xF5\xF0\xA3\x91\x0F\x1B\x3F\x84\xE9\xDE\x62\x4F\xDA\xCF\xF7\x80\xB3\xF2\x65\xDD\x49\x18\x6A\x53\xF7\xC6\x96\xEF\xBC\xBD\xFD\xAE\xC7\x70\xA9\xE0\x8A\xDF\x64\x4D\x10\x27\x9D\xE5\x4F\x76\xE8\x9E\x8A\xDA\xBC\x01\x9A\x4B\x66\xA8\xFA\xC2\xED\x80\x48\x77\x81\x84\x7F\x42\xB0\x82\x84\x29\x3B\xD9\x37\xBD\xC3\x2C\x9F\xA4\x70\x31\xD4\x59\xF9\xB2\xB6\x18\x8F\x78\x9A\x6A\xB5\x67\x94\x8E\xE3\x52\x66\xB0\x9F\x88\x4B\x26\xE2\xCF\x7E\x46\x70\x08\xB7\x80\x37\xEE\x41\x65\x04\xB7\xFA\xD2\x26\x78\x50\x58\x04\xFA\x1B\x3E\xF5\x21\xF2\xCC\xBE\x9C\x8E\x0A\x5D\x9D\xCA\xDD\x1E\x2D\xD9\x39\xBC\xE6\xAB\xD9\x62\x9C\xC3\x10\x46\x22\x36\xE2\x86\x7C\xC3\x35\x22\x48\x89\x04\x85\xC3\x2C\xBF\x5D\x0F\x4D\xB9\x41\xFD\x84\x3C\x43\x77\x4C\x62\xF7\x95\x0F\x8D\x64\x83\xB5\x6F\x49\x30\xD0\x5A\xD9\xCA\x69"
#define NKPSHash							"\x32\xD5\x27\xA1\xB1\x6E\x52\x09\x56\xE0\xD1\x89\x1E\x9B\x92\x22\x7F\x16\xD5\x76"
#define NKPSLabel							"\x4E\x00\x4B\x00\x50\x00\x61\x00\x74\x00\x63\x00\x68\x00\x65\x00\x72\x00\x20\x00\x53\x00\x65\x00\x74\x00\x74\x00\x69\x00\x6E\x00\x67\x00\x73"
#define NKPSLabelSize						35
#define NKPSRemoveTitleImage				"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
#define NKPSRemoveTitleImageSize			14
#define NKPSRemoveItemsXML					"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
#define NKPSRemoveItemsXMLSize				12
#define NKPSShadowCPartition				"\x31\x34"
#define NKPSShadowCPartitionSize			2
// Resoftmod dashboard
#define ResoftmodDashHabibi					"\x58\x42\x45\x48\xD1\x8E\xCA\x2A\x1E\x86\xE5\xF3\x7B\x59\xFB\x12\x3C\xA9\x7F\xF9\x9A\xF1\x26\x6E\xC0\xF8\x8D\x09\x16\xEA\xD2\x07\x6D\xB2\x47\x9E\x0D\x83\x01\x86\x3B\x1F\x41\x9D\x69\x53\x93\xBC\x8E\x56\x6E\xDC\x60\x55\xB3\xA7\x02\x84\xED\xAA\xA4\x09\xAF\x72\xF1\x7E\x34\x9A\x32\xCD\xA4\x8C\x06\x42\x1F\x45\xB7\x75\x3A\xE4\x37\x1F\x01\x67\x4B\xBA\x8C\xA9\x44\x44\xD6\x7E\x3E\x5C\xCC\xB5\x12\x0D\x9C\x25\x8C\x11\xF6\x51\xDD\x7D\x68\x4E\xBD\x06\xA4\xB8\x12\x69\x2A\x3E\xE8\xD7\x89\x48\xB8\x00\x53\xD7\x76\xA3\x22\xB9\x94\x79\xE1\x63\x48\x78\xD2\x4D\xF6\xB4\x8F\x3F\x84\x61\x6E\x30\x45\xF1\x42\xE0\xBF\x9E\x1E\x73\x19\x1F\xC1\x66\x81\x80\x49\xF4\xF0\x44\xD5\xF5\x78\x2F\xF8\x72\xD4\xC9\x82\xA1\x7F\xE8\x17\xE1\xE9\x34\xAB\x95\xA9\xC0\x91\x83\x86\xDB\x1A\xBB\x43\xD0\x99\x4C\x3C\xEE\xC5\x66\x67\xE8\x59\xAF\x37\x4B\xD0\x92\xB2\xEE\x9F\xBE\xBB\x73\x28\x98\xD0\x56\xBD\x7E\xAC\x7E\x91\x36\x86\xE2\xEA\xC5\x91\xEA\x3E\x7D\x37\xB7\xB7\xB9\x42\xA7\x09\x13\x6D\xC7\x3B\x3B\x34\x0A\x54\x58\xA6\x97\xA0\xC6\x3A\x09\x38\x29\x38\xB3\xA3\xFA\xCB\x36\x28\x10"
#define ResoftmodDashHash					"\x4E\xB0\x9E\xF5\x9D\x96\xC8\xF0\x32\xD6\x44\x4C\xF2\x6B\x0D\x99\xE1\x91\xCA\x26"
#define ResoftmodDashLabel					"\x52\x00\x65\x00\x53\x00\x6F\x00\x66\x00\x74\x00\x6D\x00\x6F\x00\x64\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68"
#define ResoftmodDashLabelSize				27
// C Rescue dashboard
#define CRescueDashHabibi					"\x58\x42\x45\x48\xC9\x8E\xFD\x20\x6B\x0E\x5F\x2B\x81\x2F\xB8\x3B\x6F\xA6\x3A\x5D\x5E\x30\x74\x95\xF1\x60\x1C\xC2\x97\xC4\xF2\xE7\xF9\x71\x38\x7E\xEC\xC5\x34\xE3\xD5\xC2\x33\xB5\x32\xC7\xB3\xB2\x60\x8D\x75\xBE\xF1\x47\xC1\x31\xC0\x29\xD8\x1D\x31\xD9\xCC\xF5\x75\x03\x7C\xDA\x3A\x68\x37\xEA\x0B\x22\xF0\x35\x53\x91\xCC\xFE\x18\x2A\xCF\x30\x48\x7A\xE8\x41\xFD\xA6\xF8\x42\xE7\xB0\xD8\x95\x7F\xB4\x33\xF8\x82\x9B\xED\x97\xEA\x75\x61\x59\xC5\x77\xC9\xC8\xD5\x3F\x1E\xC2\x04\x1D\xE3\xC1\x3A\x06\x68\x93\x92\xC6\x41\x86\xAF\x4E\xB7\x7E\xBC\x41\xFA\x41\x34\x2B\x85\x6C\xA1\x23\x73\xEA\xBC\x72\x53\x25\x0E\xAB\x50\x34\x23\x6F\x9E\x89\x5D\x17\x36\x84\x09\x62\xB0\xAB\x43\x92\x48\x66\x30\x11\x91\x89\x40\x31\x80\x87\x9D\x81\xA3\x86\xE4\x1D\x8F\x3E\xD4\x65\x65\x22\x96\x06\x2F\x5B\xEF\xC9\xBB\x9D\xD7\x8A\x38\x4C\x01\x90\x65\xD6\xFF\xAB\x79\xC3\x1A\x24\xF8\xFC\x4B\xF6\x38\x28\xBB\xE5\x96\xFD\x50\x1E\x09\xAE\x57\xD2\x76\x6D\xE3\x43\x28\x3D\x22\xFE\x00\x6D\x5C\x39\x16\xAD\x2A\x65\xB9\xC9\x74\x86\xD8\x63\x4D\x5A\x7D\x06\x26\xFB\xF2\x43\x88\xD1\x4B\x41"
#define CRescueDashHash						"\xC1\xA8\xD8\x0E\x52\xF5\x6E\x1C\x98\x67\xDC\x99\x35\x0D\x9B\x70\xCC\xFA\x08\xDC"
#define CRescueDashLabel					"\x43\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00"
#define CRescueDashLabelSize				29
#define CRescueDashShadowCPartition			"\x31\x34"
#define CRescueDashShadowCPartitionSize		2
// E Rescue dashboard
#define ERescueDashHabibi					"\x58\x42\x45\x48\xA3\x02\x62\x73\x45\xD1\x4E\x97\x74\x98\x57\x92\xEC\x68\xED\x3D\x6F\x36\xE9\xDE\x47\x2C\xAD\x93\xCE\x5F\x0F\x07\x53\x55\xC5\x16\xF5\x72\xF4\x73\x96\x57\xBB\x0D\xE5\x76\x91\x18\xCA\x53\x07\xDF\x3B\x47\x50\xE8\x2D\x8C\x49\xFF\x25\x3B\x82\x2D\x2A\x72\x2C\x6D\x82\xFB\x33\x6E\x0C\x74\xB9\xC9\x0A\x74\x33\x65\xAA\x7E\x9F\x1F\xC8\x88\x81\x35\x53\xD6\x1E\xF3\xA7\x8B\xBB\xF0\x8C\x21\x10\xB7\x83\xF7\x44\xE3\xBE\xF8\x48\x8E\x87\x7C\x25\xBF\x74\x5E\x53\xC3\xC2\x42\x4E\x3C\x79\xDB\x26\xC4\x75\x55\x1D\xB6\xC6\x5E\x1B\x3C\x1E\xED\x48\xF0\xEA\xED\x8F\xAA\x4F\x8B\x7C\x89\x49\x87\x3A\x29\x7B\x5B\x7E\x56\x41\x66\xC9\x2A\x27\x4C\xEC\xB1\xAA\xFE\x35\x35\xA7\x29\x44\xCA\x4C\x0B\x7E\x60\x6E\x76\xF4\x75\xD7\x42\x05\x90\x99\xF1\x4B\x18\xB1\x9C\xB6\x22\x21\x30\x3B\x47\x87\xEA\x46\xF2\x40\xBC\x3F\x29\x6A\x6D\xFA\x4D\x20\x64\x10\x74\x7C\x62\x60\x0B\x30\x1D\xD8\xD1\x1E\x80\x77\xB2\x06\x72\x58\xA4\x1B\x54\x98\x83\xD7\x0C\x5E\x1A\xC9\x24\xCD\xAC\xF9\x77\x9D\x77\xF6\x04\xFA\x1E\x7D\xC7\x53\xC9\x4D\xA1\xB2\xEF\x0C\xE0\xCD\x93\x19\x6A\x52\x87"
#define ERescueDashHash						"\x4E\xB0\x9E\xF5\x9D\x96\xC8\xF0\x32\xD6\x44\x4C\xF2\x6B\x0D\x99\xE1\x91\xCA\x26"
#define ERescueDashLabel					"\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00"
#define ERescueDashLabelSize				29
// Dashboard Settings
#define DashSettingsHabibi					"\x58\x42\x45\x48\xE2\x3A\x7C\x9C\xFD\xBA\x74\x30\xD8\xB8\x0E\x57\x0A\x19\x95\x37\x96\xA5\xFB\xF4\x86\x73\x7B\x03\x78\xDA\x00\x68\xCC\x4D\xBE\xCD\xD4\xB3\x7B\x68\x00\xD1\x6C\xA8\xF9\xB7\x89\x71\x72\xBB\xA8\xDA\x90\xFA\xD7\xDC\xFE\xAC\xA5\x03\xD1\xCE\x3A\xBD\x3C\x72\x72\xEC\x2F\xC3\x6C\xC0\xF4\x95\xC0\x7E\x37\x75\xE6\xA6\x2F\xC2\x02\x97\xA4\x01\xF9\x23\xA4\xE4\x9F\x0F\x28\x78\x74\x42\xA0\x55\x56\x68\xA9\xDC\x39\xE5\xC9\x25\xBA\x20\x29\x7C\xE3\x60\x82\x8D\x79\x1F\x5C\xFD\xD3\xF6\xDC\x2A\x9F\x73\xF5\x82\x47\x5E\x34\x36\xA3\x45\x07\xE9\xD0\x1C\x58\x1B\xF2\xDB\xE9\xBF\xA2\xB1\x7E\x03\x83\x55\xBC\x36\x7B\x72\x47\xF6\x45\xED\x78\x24\x76\x50\xB9\x97\xAA\xD5\x6A\xD4\x87\xE5\x75\x83\x99\x89\x78\xFF\x8B\x7F\x17\x08\x01\x77\x39\x4A\xD7\x6F\x3B\xF6\x2E\xFF\x29\xDD\x9F\x68\xCB\xD7\xBB\x98\x6C\xAF\xAB\x81\xBA\x7B\x5F\x61\x88\x6F\x18\xD9\x05\x72\xF3\x17\x6E\xAD\xF0\xF6\x5C\x2D\x1A\x0F\x06\xE5\x97\x30\x75\x2C\xC0\x00\xCF\x02\xFA\x69\x00\xCD\x0F\x95\xE3\xD4\x72\xF2\xDA\x55\x6D\x39\x6C\xEC\xFC\xDC\x0B\x9E\x5E\x8E\x8B\xFC\xDE\x14\x58\x8B\x12\x84"
#define DashSettingsHash					"\x4E\xB0\x9E\xF5\x9D\x96\xC8\xF0\x32\xD6\x44\x4C\xF2\x6B\x0D\x99\xE1\x91\xCA\x26"
#define DashSettingsLabel					"\x42\x00\x61\x00\x63\x00\x6B\x00\x75\x00\x70\x00\x2C\x00\x20\x00\x52\x00\x65\x00\x73\x00\x74\x00\x6F\x00\x72\x00\x65\x00\x20\x00\x6F\x00\x72\x00\x20\x00\x4D\x00\x6F\x00\x76\x00\x65"
#define DashSettingsLabelSize				45
// All XBE files
#define RemoveRecentdat						"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
#define RemoveRecentdatSize					10
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Checksum3 length is 0x60 - do not NULL to FF or you will brick your Xbox
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CheckSum3NULL						"FFFFFFFF0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LED Colours
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	LED_Orange							XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE1_GREEN|XKUtils::LED_REGISTER_CYCLE1_RED|XKUtils::LED_REGISTER_CYCLE2_GREEN|XKUtils::LED_REGISTER_CYCLE2_RED|XKUtils::LED_REGISTER_CYCLE3_GREEN|XKUtils::LED_REGISTER_CYCLE3_RED)
#define LED_Flash_Orange					XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE3_GREEN|XKUtils::LED_REGISTER_CYCLE3_RED)
#define LED_Green							XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE1_GREEN|XKUtils::LED_REGISTER_CYCLE2_GREEN|XKUtils::LED_REGISTER_CYCLE3_GREEN)
#define LED_Flash_Green						XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE3_GREEN)
#define LED_Flash_Green_Orange				XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE1_GREEN|XKUtils::LED_REGISTER_CYCLE1_RED|XKUtils::LED_REGISTER_CYCLE2_GREEN|XKUtils::LED_REGISTER_CYCLE3_GREEN)
#define LED_Flash_Red						XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE3_RED)
#define LED_Flash_Red_Fast					XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE2_RED)
#define LED_Flash_Red_Orange				XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE1_GREEN|XKUtils::LED_REGISTER_CYCLE1_RED|XKUtils::LED_REGISTER_CYCLE2_RED|XKUtils::LED_REGISTER_CYCLE3_RED)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Retail Bios Stuff
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Retail_Bios_Hash_File								"D:\\Media\\RetailMD5Hashes.ini"
#define Retail_Bios_Save_Path								"E:\\Backups\\BIOS\\"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigMagicApp::VirtualEEPROMRemoval()
{
	if (file_exist(Virtual_Remove_File))
	{
		Sleep(1000);
		remove(Virtual_Full_Path);
		remove("S:\\NKPatcher\\Configs\\EEPROM_off.bin");
		remove(Virtual_Remove_File);
		remove(RNKP_File);
		remove(PrepXBE);
		RemoveDirectory(PrepDir);
		XKUtils::LaunchXBE(NKPatcherSettings);
	}
}

void ConfigMagicApp::EnableVirtualEEPROM()
{
	if (file_exist(Virtual_File))
	{
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("INSTALLING VIRTUAL EEPROM");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
		Render();
		Sleep(1000);
		remove(Virtual_File);
		remove(Virtual_Full_Path);
		remove("S:\\NKPatcher\\Configs\\EEPROM_off.bin");
		remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\veeprom\\disabled.png");
		CopyFile(EnabledPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\veeprom\\enabled.png", NULL);
		//Create Full path for BIN File..
		CHAR tmpFileName[FILENAME_MAX];
		ZeroMemory(tmpFileName, FILENAME_MAX);
		strcat(tmpFileName, Virtual_Full_Path);
		V_Serial();
		m_pXKEEPROM->WriteToBINFile((LPCSTR)tmpFileName);
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Complete");
		Render();
		Sleep(1500);
		SetupRNKP();
		XKUtils::XBOXReset();
	}
}

void ConfigMagicApp::SetUpDashboard()
{
	if (file_exist(SecondRun))
	{
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("INSTALLING DASHBOARD FILES");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
		Render();
		Sleep(1000);
		remove(FirstRunBin);
		remove(SecondRun);
		remove(PrepXBE);
		RemoveDirectory(PrepDir);
		CreateDirectory("C:\\Dashboard", NULL);
		CreateDirectory("C:\\Dashboard\\Skins", NULL);
		CreateDirectory("C:\\Dashboard\\Skins\\Softmod", NULL);
		CreateDirectory("C:\\Dashboard\\Skins\\Softmod Wide", NULL);
		CreateDirectory("C:\\Dashboard\\Skins\\Softmod Evox", NULL);
		CopyFile(DashboardXBE, "C:\\Dashboard\\default.xbe", NULL);
		CopyFile(DashboardXML, "C:\\Dashboard\\config.xml", NULL);
		CopyFile(DashboardSkinXML, "C:\\Dashboard\\Skins\\Softmod\\skin.xml", NULL);
		CopyFile(DashboardSkinwideXML, "C:\\Dashboard\\Skins\\Softmod Wide\\skin.xml", NULL);
		CopyFile(DashboardSkinevoxXML, "C:\\Dashboard\\Skins\\Softmod Evox\\skin.xml", NULL);
		CopyFile(DashboardSkinSplash, "C:\\Dashboard\\Skins\\Softmod\\UXSplash.jpg", NULL);
		CopyFile(DashboardSkinSplash, "C:\\Dashboard\\Skins\\Softmod Wide\\UXSplash.jpg", NULL);
		CopyFile(DashboardSkinSplash, "C:\\Dashboard\\Skins\\Softmod Evox\\UXSplash.jpg", NULL);
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Complete");
		Render();
		Sleep(1000);
		if (file_exist(Dashloader))
		{
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Loading Dashboard");
			Render();
			Sleep(1500);
			XKUtils::LaunchXBE(Dashloader);
		}
		else
		{
			((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("ERROR !!");
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Loading Recovery Menu");
			Render();
			LED_Flash_Red;
			Sleep(3000);
			XKUtils::LaunchXBE(Rescuedashloader);
		}
	}
	if (file_exist(SecondRunUpdate))
	{
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("UPDATING DASHBOARD FILES");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
		Render();
		Sleep(1000);
		remove(SecondRunUpdate);
		remove(PrepXBE);
		RemoveDirectory(PrepDir);
		if (file_exist("C:\\dashboard\\skins\\Softmod\\skin.xml"))
		{
			CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\skins\\dashboard\\skin.xml", "C:\\dashboard\\skins\\Softmod\\skin.xml", NULL);
			CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\skins\\dashboard\\skinevox.xml", "C:\\dashboard\\skins\\Softmod Evox\\skin.xml", NULL);
			CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\skins\\dashboard\\skinwide.xml", "C:\\dashboard\\skins\\Softmod Wide\\skin.xml", NULL);
			if (file_exist("C:\\dashboard\\config.xml"))
			{
				std::rename("C:\\dashboard\\config.xml", "C:\\dashboard\\config_old.xml");
				CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\configs\\dash.xml", "C:\\dashboard\\config.xml", NULL);
				CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\configs\\alt xbe's\\dashboard.xbe", "C:\\dashboard\\default.xbe", NULL);
			}
		}
		if (file_exist("E:\\dashboard\\skins\\Softmod\\skin.xml"))
		{
			CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\skins\\dashboard\\skin.xml", "E:\\dashboard\\skins\\Softmod\\skin.xml", NULL);
			CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\skins\\dashboard\\skinevox.xml", "E:\\dashboard\\skins\\Softmod Evox\\skin.xml", NULL);
			CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\skins\\dashboard\\skinwide.xml", "E:\\dashboard\\skins\\Softmod Wide\\skin.xml", NULL);
			if (file_exist("E:\\dashboard\\config.xml"))
			{
				std::rename("E:\\dashboard\\config.xml", "E:\\dashboard\\config_old.xml");
				CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\configs\\dash.xml", "E:\\dashboard\\config.xml", NULL);
				CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\configs\\alt xbe's\\dashboard.xbe", "E:\\dashboard\\default.xbe", NULL);
			}
		}
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Complete");
		Render();
		Sleep(1000);
		if (file_exist(Dashloader))
		{
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Loading Dashboard");
			Render();
			Sleep(1500);
			XKUtils::LaunchXBE(Dashloader);
		}
		else
		{
			((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("ERROR !!");
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Loading Recovery Menu");
			Render();
			LED_Flash_Red;
			Sleep(3000);
			XKUtils::LaunchXBE(Rescuedashloader);
		}
	}
}

void ConfigMagicApp::Stage2OfInstallSoftmod()
{
	if (file_exist(FirstRunBin))
	{
		CopyFile(FirstRunBin, SecondRun, NULL);
		remove(ShadowCOFF);
		remove(FirstRunBin);
		remove(ShadowC_Location);
		remove("E:\\UDATA\\9e115330\\0064122817A8\\recent.dat");
		CopyFile(EnabledPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png", NULL);
		{
			((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("CREATING SHADOWC PARTITION");
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
			Render();
			Sleep(1500);
			/* Create ShadowC.img */
			int i;
			std::ofstream FatxHeaderFile(ShadowC_Location, std::ios::binary);
			for(i = 0; i < sizeof(fatxheader); i++)
			{
				FatxHeaderFile << fatxheader[i];
			}
			FatxHeaderFile.close();
			
			std::ofstream ShadowCFile(ShadowC_Location, std::ios::binary | std::ios::out);
			ShadowCFile.seekp((ShadowC_Size<<20) - 1);
			ShadowCFile.write("", 1);
			ShadowCFile.close();
			// Check if shadowc.img is written and if not write a smaller one.
			FILE *fp;
			fp = fopen( ShadowC_Location,"r" );
			fseek(fp, 0, SEEK_END); // goto end of file
			if (ftell(fp) == 0)
			{
				fclose(fp);
				int i;
				std::ofstream FatxHeaderFile(ShadowC_Location, std::ios::binary);
				for(i = 0; i < sizeof(fatxheader); i++)
				{
					FatxHeaderFile << fatxheader[i];
				}
				FatxHeaderFile.close();
				
				std::ofstream ShadowCFileAlt(ShadowC_Location, std::ios::binary | std::ios::out);
				ShadowCFileAlt.seekp((ShadowC_Size_Alt<<20) - 1);
				ShadowCFileAlt.write("", 1);
				ShadowCFileAlt.close();
			}
		}
		ColdBootSoftmod();
		PatchXBEFiles();
		//InstallKernelFont();
		CreateEEPROMBackup();
		XKUtils::XBOXReset();
	}
}

void ConfigMagicApp::Stage2OfUpdateSoftmod()
{
	if (file_exist(UpdateRunBin))
	{
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("UPDATING SOFTMOD");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
		Render();
		Sleep(1500);
		remove(ShadowCOFF);
		remove(UpdateRunBin);
		CopyFile(Dummy_File, SecondRunUpdate, NULL);
		remove("E:\\UDATA\\9e115330\\0064122817A8\\recent.dat");
		CopyFile(EnabledPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png", NULL);
		ColdBootSoftmod();
		PatchXBEFiles();
		//InstallKernelFont();
		CreateEEPROMBackup();
		XKUtils::XBOXReset();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigMagicApp::InstallKernelFont()
{
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("INSTALLING KERNEL FONT");
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
	Render();
	Sleep(1000);
	KernelFontInstall();
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Complete");
	Render();
	Sleep(1000);
}

void ConfigMagicApp::UpdateKernelFont()
{
	if (file_exist(Update_Font))
	{
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("UPDATING INSTALLED KERNEL FONT");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
		Render();
		Sleep(1000);
		KernelFontInstall();
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Complete");
		Render();
		Sleep(1500);
		XKUtils::LaunchXBE(NKPatcherSettings);
	}
}

void ConfigMagicApp::RestoreKernelFont()
{
	if (file_exist(Restore_Font))
	{
		if (file_exist(Generic_Font_File_Alt))
		{
			remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png");
			remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.3944.1.png");
			remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4034.1.png");
			remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4627.1.png");
			remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4817.1.png");
			remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5101.1.png");
			remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5530.1.png");
			remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5713.1.png");
			remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5838.1.png");
			remove(xbox_xtf_File_Alt);
			remove(Restore_Font);
			CopyFile(Generic_Font_File_Alt, xbox_xtf_File_Alt, 1);
			CopyFile(EnabledPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png", NULL);
		}
		XKUtils::LaunchXBE(NKPatcherSettings);
	}
}

void ConfigMagicApp::ReloadNKPatcherSettings()
{
	if (file_exist(RNKP_File))
	{
		remove(PrepXBE);
		remove(RNKP_File);
		RemoveDirectory(PrepDir);
		XKUtils::LaunchXBE(NKPatcherSettings);
	}
}

void ConfigMagicApp::NullHardDriveKey()
{
	if (file_exist(NULLHDDKey_File))
	{
		remove(NULLHDDKey_File);
		UnLock_New_HDD();
		NULL_HDD_KEY();
		Write_XBOX_EEPROM();
		Lock_New_HDD();
		if (file_exist("E:\\Backups\\eeprom\\eeprom_original.bin"))
		{
			CreateEEPROMBackup();
		}
		else
		{
			std::rename("E:\\Backups\\eeprom\\eeprom.bin", "E:\\Backups\\eeprom\\eeprom_original.bin");
			CreateEEPROMBackup();
		}
		XKUtils::LaunchXBE(NKPatcherSettings);
	}
}

void ConfigMagicApp::LockHardDrive()
{
	if (file_exist(LockHDD_File))
	{
		CreateEEPROMBackup();
		Lock_New_HDD();
		XKUtils::LaunchXBE(PrepUX);
	}
}
void ConfigMagicApp::ChangeVideoRegionToNTSC()
{
	if (file_exist(Region_File1))
	{
		remove(Region_File1);
		ChangeRegionNTSC();
		SetupRNKP();
		XKUtils::XBOXReset();
	}
}

void ConfigMagicApp::ChangeVideoRegionToPAL()
{
	if (file_exist(Region_File2))
	{
		remove(Region_File2);
		ChangeRegionPAL();
		SetupRNKP();
		XKUtils::XBOXReset();
	}
}

void ConfigMagicApp::NULLEEPROMCheckSum3()
{
	if (file_exist(NullChecksum3_File))
	{
		remove(NullChecksum3_File);
		NULLChecksum3();
		XKUtils::XBOXPowerCycle();
	}
}

void ConfigMagicApp::NTSCResetToFactoryEEPROM()
{
	if (file_exist(FactoryResetNTSC_File))
	{
		remove(FactoryResetNTSC_File);
		FactoryResetNTSC();
		remove(PrepXBE);
		RemoveDirectory(PrepDir);
		XKUtils::XBOXReset();
	}
}

void ConfigMagicApp::PALResetToFactoryEEPROM()
{
	if (file_exist(FactoryResetPAL_File))
	{
		remove(FactoryResetPAL_File);
		FactoryResetPAL();
		remove(PrepXBE);
		RemoveDirectory(PrepDir);
		XKUtils::XBOXReset();
	}
}

void ConfigMagicApp::PatchNKPatcherToSupportOtherPartitions()
{
	if (file_exist(PatchNKP_File1))
	{
		remove(PatchNKP_File1);
		if (file_exist("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\part\\default-part.png"))
		{
			if (file_exist(PatchNKP_File2))
			{
				remove(PatchNKP_File2);
				((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("DISABLING DUAL BOOTING");
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
			}
			else
			{
				((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("ENABLING ONLY F PARTITION");
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
			}
			Render();
			Sleep(1000);
			std::ofstream NKPDefaultF(NKP_XBE, std::ios::binary | std::ios::in | std::ios::out);
			// Habibi Signature
			NKPDefaultF.seekp(0);
			NKPDefaultF.write(NKPHabibi_F_No_DB, 260);
			// XBE Hash
			NKPDefaultF.seekp(888);
			NKPDefaultF.write(NKPHash_F_No_DB, 20);
			//
			NKPDefaultF.seekp(8027);
			NKPDefaultF.write(NKPValue1, NKPValue1Size);
			//
			NKPDefaultF.seekp(5700);
			NKPDefaultF.write(NKPValue3, NKPValue1Size);
			//
			NKPDefaultF.seekp(17080);
			NKPDefaultF.write(NKPValue1, NKPValue1Size);
			NKPDefaultF.close();
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Complete");
		}
		if (file_exist("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\part\\default-FG.png"))
		{
			if (file_exist(PatchNKP_File2))
			{
				remove(PatchNKP_File2);
				((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("DISABLING DUAL BOOTING");
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
			}
			else
			{
				((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("ENABLING F & G PARTITION");
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
			}
			Render();
			Sleep(1000);
			std::ofstream NKPDefaultFG(NKP_XBE, std::ios::binary | std::ios::in | std::ios::out);
			// Habibi Signature
			NKPDefaultFG.seekp(0);
			NKPDefaultFG.write(NKPHabibi_F_G_No_DB, 260);
			// XBE Hash
			NKPDefaultFG.seekp(888);
			NKPDefaultFG.write(NKPHash_F_G_No_DB, 20);
			//
			NKPDefaultFG.seekp(8027);
			NKPDefaultFG.write(NKPValue2, NKPValue1Size);
			//
			NKPDefaultFG.seekp(5700);
			NKPDefaultFG.write(NKPValue3, NKPValue1Size);
			//
			NKPDefaultFG.seekp(17080);
			NKPDefaultFG.write(NKPValue2, NKPValue1Size);
			NKPDefaultFG.close();
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Complete");
		}
		Render();
		Sleep(1500);
		SetupRNKP();
		XKUtils::XBOXReset();
	}
}

void ConfigMagicApp::PatchNKPatcherToSupportDualBoot()
{
	if (file_exist(PatchNKP_File3))
	{
		remove(PatchNKP_File3);
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("ENABLING DUAL BOOTING");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
		Render();
		Sleep(1000);
		if (file_exist("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\part\\default-part.png"))
		{
			std::ofstream NKPDefaultFDB(NKP_XBE, std::ios::binary | std::ios::in | std::ios::out);
			// Habibi Signature
			NKPDefaultFDB.seekp(0);
			NKPDefaultFDB.write(NKPHabibi_F_DB, 260);
			// XBE Hash
			NKPDefaultFDB.seekp(888);
			NKPDefaultFDB.write(NKPHash_F_DB, 20);
			//
			NKPDefaultFDB.seekp(8027);
			NKPDefaultFDB.write(NKPValue1, NKPValue1Size);
			//
			NKPDefaultFDB.seekp(5700);
			NKPDefaultFDB.write(NKPValue4, NKPValue1Size);
			//
			NKPDefaultFDB.seekp(17080);
			NKPDefaultFDB.write(NKPValue1, NKPValue1Size);
			NKPDefaultFDB.close();
		}
		if (file_exist("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\part\\default-FG.png"))
		{
			std::ofstream NKPDefaultFGDB(NKP_XBE, std::ios::binary | std::ios::in | std::ios::out);
			// Habibi Signature
			NKPDefaultFGDB.seekp(0);
			NKPDefaultFGDB.write(NKPHabibi_F_G_DB, 260);
			// XBE Hash
			NKPDefaultFGDB.seekp(888);
			NKPDefaultFGDB.write(NKPHash_F_G_DB, 20);
			//
			NKPDefaultFGDB.seekp(8027);
			NKPDefaultFGDB.write(NKPValue2, NKPValue1Size);
			//
			NKPDefaultFGDB.seekp(5700);
			NKPDefaultFGDB.write(NKPValue4, NKPValue1Size);
			//
			NKPDefaultFGDB.seekp(17080);
			NKPDefaultFGDB.write(NKPValue2, NKPValue1Size);
			NKPDefaultFGDB.close();
		}
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Complete");
		Render();
		Sleep(1500);
		SetupRNKP();
		XKUtils::XBOXReset();
	}
}

void ConfigMagicApp::DumpDecryptedEEPROM()
{
	if (file_exist(DecryptedEERPOM_File))
	{
		CreateDecryptedEEPROMBackup();
		XKUtils::LaunchXBE(NKPatcherSettings);
	}
}

void ConfigMagicApp::EnablePersistentSoftmodState()
{
	if (file_exist(EnableControlledError16_File))
	{
		remove(EnableControlledError16_File);
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("ENABLING PERSISTENT SOFTMOD");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
		Render();
		Sleep(2000);
		EnableControlledError16State();
		XKUtils::XBOXReset();
	}
}

void ConfigMagicApp::DisablePersistentSoftmodState()
{
	if (file_exist(DisableControlledError16_File))
	{
		remove(DisableControlledError16_File);
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("DISABLING THE PERSISTENT SOFTMOD");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
		Render();
		Sleep(2000);
		DisableControlledError16State();
		XKUtils::XBOXReset();
	}
}

void ConfigMagicApp::RemoveParentalControls()
{
	if (file_exist(Remove_Parental_Controls))
	{
		remove(Remove_Parental_Controls);
		//On Screen Text
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("REMOVING PARENTAL CONTROLS");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
		//Refresh screen....
		Render();
		Sleep(1000);
		m_pXKEEPROM->ReadFromXBOX();
		m_pXKEEPROM->ParentalControlGamesString("00000000");
		m_pXKEEPROM->ParentalControlPWDString("00000000");
		m_pXKEEPROM->ParentalControlMoviesString("00000000");
		Force_Write_XBOX_EEPROM();
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Complete");
		Render();
		Sleep(1000);
		XKUtils::XBOXReset();
	}
}

void ConfigMagicApp::SetLanguage()
{
	if (file_exist(Change_Language_File))
	{
		remove(Change_Language_File);
		//On Screen Text
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("CHANGING SYSTEM LANGUAGE");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Patching EEPROM Data");
		//Refresh screen....
		Render();
		Sleep(1000);
	}
	if (file_exist(English_File))
	{
		remove(English_File);
		m_pXKEEPROM->ReadFromXBOX();
		m_pXKEEPROM->SetLanguageString("01");
		Force_Write_XBOX_EEPROM();
		XKUtils::XBOXReset();
	}
	if (file_exist(Japanese_File))
	{
		remove(Japanese_File);
		m_pXKEEPROM->ReadFromXBOX();
		m_pXKEEPROM->SetLanguageString("02");
		Force_Write_XBOX_EEPROM();
		XKUtils::XBOXReset();
	}
	if (file_exist(German_File))
	{
		remove(German_File);
		m_pXKEEPROM->ReadFromXBOX();
		m_pXKEEPROM->SetLanguageString("03");
		Force_Write_XBOX_EEPROM();
		XKUtils::XBOXReset();
	}
	if (file_exist(French_File))
	{
		remove(French_File);
		m_pXKEEPROM->ReadFromXBOX();
		m_pXKEEPROM->SetLanguageString("04");
		Force_Write_XBOX_EEPROM();
		XKUtils::XBOXReset();
	}
	if (file_exist(Spanish_File))
	{
		remove(Spanish_File);
		m_pXKEEPROM->ReadFromXBOX();
		m_pXKEEPROM->SetLanguageString("05");
		Force_Write_XBOX_EEPROM();
		XKUtils::XBOXReset();
	}
	if (file_exist(Italian_File))
	{
		remove(Italian_File);
		m_pXKEEPROM->ReadFromXBOX();
		m_pXKEEPROM->SetLanguageString("06");
		Force_Write_XBOX_EEPROM();
		XKUtils::XBOXReset();
	}
	if (file_exist(Korean_File))
	{
		remove(Korean_File);
		m_pXKEEPROM->ReadFromXBOX();
		m_pXKEEPROM->SetLanguageString("07");
		Force_Write_XBOX_EEPROM();
		XKUtils::XBOXReset();
	}
	if (file_exist(Chinese_File))
	{
		remove(Chinese_File);
		m_pXKEEPROM->ReadFromXBOX();
		m_pXKEEPROM->SetLanguageString("08");
		Force_Write_XBOX_EEPROM();
		XKUtils::XBOXReset();
	}
	if (file_exist(Portuguese_File))
	{
		remove(Portuguese_File);
		m_pXKEEPROM->ReadFromXBOX();
		m_pXKEEPROM->SetLanguageString("09");
		Force_Write_XBOX_EEPROM();
		XKUtils::XBOXReset();
	}
}

void ConfigMagicApp::PatchXBEFiles()
{
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("MOVING XBE FILES");
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
	Render();
	// Copy UX xbe files to there right place
	remove(ResoftmodDash);
	remove(ERescueDash1);
	remove(ERescueDash2);
	remove(DashSettings);
	remove(CRescueDash);
	remove(DashboardXBE);
	CopyFile(UnleashXXBE, NKPatcherSettings, NULL);
	CopyFile(UnleashXXBE, ResoftmodDash, NULL);
	CopyFile(UnleashXXBE, ERescueDash1, NULL);
	CopyFile(UnleashXXBE, ERescueDash2, NULL);
	CopyFile(UnleashXXBE, DashSettings, NULL);
	CopyFile(UnleashXXBE, CRescueDash, NULL);
	CopyFile(UnleashXXBE, DashboardXBE, NULL);
	
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("PATCHING XBE FILES");
	Render();
	// Patch UnleashX to show unprotected C partition for NKPatcher Settings
	std::ofstream UnleashX_XBE1(NKPatcherSettings, std::ios::binary | std::ios::in | std::ios::out);
	// Habibi Signature
	UnleashX_XBE1.seekp(0);
	UnleashX_XBE1.write(NKPSHabibi, 260);
	// Change XBE Title ( NKPatcher Settings )
	UnleashX_XBE1.seekp(400);
	UnleashX_XBE1.write(NKPSLabel, NKPSLabelSize);
	// XBE Hash
	UnleashX_XBE1.seekp(1532);
	UnleashX_XBE1.write(NKPSHash, 20);
	// Remove recent.dat creation
	UnleashX_XBE1.seekp(2353004);
	UnleashX_XBE1.write(RemoveRecentdat, RemoveRecentdatSize);
	// Remove XBE TitleImage.xbx parsing
	UnleashX_XBE1.seekp(2353334);
	UnleashX_XBE1.write(NKPSRemoveTitleImage, NKPSRemoveTitleImageSize);
	// Remove Items.xml
	UnleashX_XBE1.seekp(2355128);
	UnleashX_XBE1.write(NKPSRemoveItemsXML, NKPSRemoveItemsXMLSize);
	// Change C partition number
	UnleashX_XBE1.seekp(2399875);
	UnleashX_XBE1.write(NKPSShadowCPartition, NKPSShadowCPartitionSize);
	UnleashX_XBE1.close();

	std::ofstream UnleashX_XBE2(ResoftmodDash, std::ios::binary | std::ios::in | std::ios::out);
	// Habibi Signature
	UnleashX_XBE2.seekp(0);
	UnleashX_XBE2.write(ResoftmodDashHabibi, 260);
	// Change XBE Title ( Resoftmod Dash )
	UnleashX_XBE2.seekp(400);
	UnleashX_XBE2.write(ResoftmodDashLabel, ResoftmodDashLabelSize);
	// XBE Hash
	UnleashX_XBE2.seekp(1532);
	UnleashX_XBE2.write(ResoftmodDashHash, 20);
	// Remove recent.dat creation
	UnleashX_XBE2.seekp(2353004);
	UnleashX_XBE2.write(RemoveRecentdat, RemoveRecentdatSize);
	UnleashX_XBE2.close();

	std::ofstream UnleashX_XBE3(CRescueDash, std::ios::binary | std::ios::in | std::ios::out);
	// Habibi Signature
	UnleashX_XBE3.seekp(0);
	UnleashX_XBE3.write(CRescueDashHabibi, 260);
	// Change XBE Title ( C Recovery Dash )
	UnleashX_XBE3.seekp(400);
	UnleashX_XBE3.write(CRescueDashLabel, CRescueDashLabelSize);
	// XBE Hash
	UnleashX_XBE3.seekp(1532);
	UnleashX_XBE3.write(CRescueDashHash, 20);
	// Remove recent.dat creation
	UnleashX_XBE3.seekp(2353004);
	UnleashX_XBE3.write(RemoveRecentdat, RemoveRecentdatSize);
	// Change C partition number
	UnleashX_XBE3.seekp(2399875);
	UnleashX_XBE3.write(CRescueDashShadowCPartition, CRescueDashShadowCPartitionSize);
	UnleashX_XBE3.close();

	std::ofstream UnleashX_XBE5(ERescueDash1, std::ios::binary | std::ios::in | std::ios::out);
	// Habibi Signature
	UnleashX_XBE5.seekp(0);
	UnleashX_XBE5.write(ERescueDashHabibi, 260);
	// Change XBE Title ( E Recovery Dash )
	UnleashX_XBE5.seekp(400);
	UnleashX_XBE5.write(ERescueDashLabel, ERescueDashLabelSize);
	// XBE Hash
	UnleashX_XBE5.seekp(1532);
	UnleashX_XBE5.write(ERescueDashHash, 20);
	// Remove recent.dat creation
	UnleashX_XBE5.seekp(2353004);
	UnleashX_XBE5.write(RemoveRecentdat, RemoveRecentdatSize);
	UnleashX_XBE5.close();

	std::ofstream UnleashX_XBE6(ERescueDash2, std::ios::binary | std::ios::in | std::ios::out);
	// Habibi Signature
	UnleashX_XBE6.seekp(0);
	UnleashX_XBE6.write(ERescueDashHabibi, 260);
	// Change XBE Title ( E Recovery Dash )
	UnleashX_XBE6.seekp(400);
	UnleashX_XBE6.write(ERescueDashLabel, ERescueDashLabelSize);
	// XBE Hash
	UnleashX_XBE6.seekp(1532);
	UnleashX_XBE6.write(ERescueDashHash, 20);
	// Remove recent.dat creation
	UnleashX_XBE6.seekp(2353004);
	UnleashX_XBE6.write(RemoveRecentdat, RemoveRecentdatSize);
	UnleashX_XBE6.close();

	std::ofstream UnleashX_XBE7(DashSettings, std::ios::binary | std::ios::in | std::ios::out);
	// Habibi Signature
	UnleashX_XBE7.seekp(0);
	UnleashX_XBE7.write(DashSettingsHabibi, 260);
	// Change XBE Title ( Backup, Restore or Move )
	UnleashX_XBE7.seekp(400);
	UnleashX_XBE7.write(DashSettingsLabel, DashSettingsLabelSize);
	// XBE Hash
	UnleashX_XBE7.seekp(1532);
	UnleashX_XBE7.write(DashSettingsHash, 20);
	// Remove recent.dat creation
	UnleashX_XBE7.seekp(2353004);
	UnleashX_XBE7.write(RemoveRecentdat, RemoveRecentdatSize);
	UnleashX_XBE7.close();
	
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Complete");
	Render();
	Sleep(1500);
}

void ConfigMagicApp::ColdBootSoftmod()
{
	//On Screen Text
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("PATCHING EEPROM DATA");
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
	//Refresh screen....
	Render();
	Sleep(1000);
	m_pXKEEPROM->ReadFromXBOX();
	//m_pXKEEPROM->SetTimeZoneStdNameString("GMT"); // Set timezone to GMT
	//m_pXKEEPROM->SetTimeZoneDltNameString("BST"); // Set Daylight saving to BST ( YES )
	//m_pXKEEPROM->SetTimeZoneStdDateString("0A050002"); // Set timezone standard Date
	//m_pXKEEPROM->SetTimeZoneDltDateString("03050001"); // Set Daylight saving Date
	//m_pXKEEPROM->SetTimeZoneDltBiasString("C4FFFFFF"); // Set Daylight saving Bias
	m_pXKEEPROM->SetVideoFlagsString("00004A00"); // Set Videoflags to (0A) 480p, 720p or (4A) PAL60, 480p and 720p
	//m_pXKEEPROM->SetLanguageString("00"); // invalid language set, this will force the xbox to boot xboxdash.xbe always
	Force_Write_XBOX_EEPROM();
}

void ConfigMagicApp::EnableControlledError16State()
{
	//On Screen Text
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("PATCHING EEPROM DATA");
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
	//Refresh screen....
	Render();
	Sleep(1000);
	m_pXKEEPROM->ReadFromXBOX();
	m_pXKEEPROM->SetLanguageString("00"); // invalid language set, this will force the xbox to boot xboxdash.xbe always
	Force_Write_XBOX_EEPROM();
}

void ConfigMagicApp::DisableControlledError16State()
{
	//On Screen Text
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("PATCHING EEPROM DATA");
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
	//Refresh screen....
	Render();
	Sleep(1000);
	m_pXKEEPROM->ReadFromXBOX();
	m_pXKEEPROM->SetLanguageString("01"); // sets language to ENGLISH and stops the persistent softmod effect
	Force_Write_XBOX_EEPROM();
}

void ConfigMagicApp::NULLChecksum3()
{
	//On Screen Text
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("NULLING CHECKSUM 3");
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
	//Refresh screen....
	Render();
	Sleep(1000);
	m_pXKEEPROM->ReadFromXBOX();
	m_pXKEEPROM->SetNullChecksum3String(CheckSum3NULL);
	Force_Write_XBOX_EEPROM();
}

void ConfigMagicApp::FactoryResetPAL()
{
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("RESETTING EEPROM");
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
	Render();
	Sleep(1000);
	m_pXKEEPROM->ReadFromXBOX();
	m_pXKEEPROM->SetVideoStandardVal(XKEEPROM::PAL_I);
	//m_pXKEEPROM->SetVideoFlagsString("00004800"); // Set Videoflags to PAL 60
	m_pXKEEPROM->SetNullChecksum3String(CheckSum3NULL);
	Force_Write_XBOX_EEPROM();
}

void ConfigMagicApp::FactoryResetNTSC()
{
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("RESETTING EEPROM");
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
	Render();
	Sleep(1000);
	m_pXKEEPROM->ReadFromXBOX();
	m_pXKEEPROM->SetVideoStandardVal(XKEEPROM::NTSC_M);
	m_pXKEEPROM->SetNullChecksum3String(CheckSum3NULL);
	Force_Write_XBOX_EEPROM();
}

void ConfigMagicApp::ChangeRegionNTSC()
{
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("CHANGING VIDEO REGION TO NTSC");
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
	Render();
	Sleep(1000);
	m_pXKEEPROM->ReadFromXBOX();
	//Re-Encrypt with correct Version...
	m_pXKEEPROM->SetVideoStandardVal(XKEEPROM::NTSC_M);
	Force_Write_XBOX_EEPROM();
}

void ConfigMagicApp::ChangeRegionPAL()
{
	//On Screen Text
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("CHANGING VIDEO REGION TO PAL");
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
	//Refresh screen....
	Render();
	Sleep(1000);
	m_pXKEEPROM->ReadFromXBOX();
	//Re-Encrypt with correct Version...
	m_pXKEEPROM->SetVideoStandardVal(XKEEPROM::PAL_I);
	Force_Write_XBOX_EEPROM();
}

void ConfigMagicApp::V_Serial()
{
	m_pXKEEPROM->ReadFromXBOX();
	m_pXKEEPROM->SetSerialNumberString("V-EEPROM ENB");
}

void ConfigMagicApp::NULL_HDD_KEY()
{	
	m_pXKEEPROM->ReadFromXBOX();
	if (file_exist("D:\\nulltestfile"))
	{
		m_pXKEEPROM->SetHDDKeyString("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	}
	else
	{
		m_pXKEEPROM->SetHDDKeyString("11111111111111111111111111111111");
	}
}

void ConfigMagicApp::CreateDecryptedEEPROMBackup()
{
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("BACKING UP DECRYPTED XBOX EEPROM");
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
	Render();
	LED_Flash_Red_Fast;

	//Read XBOX EEPROM and save to disk..
	m_pXKEEPROM->ReadFromXBOX();
	m_pXKEEPROM->Decrypt();
	
	//incase path is on these drives..
	XKUtils::MountDiskE();
	CHAR tmpFileName[FILENAME_MAX];
	
	//Create Full path for BIN File..
	ZeroMemory(tmpFileName, FILENAME_MAX);
	strcat(tmpFileName, "D:\\Decrypted_EEPROM.bin");
	remove((LPCSTR)tmpFileName);
	m_pXKEEPROM->DWriteToBINFile((LPCSTR)tmpFileName);

	XKUtils::UnMountDiskE();
	//On Screen Text
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Complete");
	//Refresh screen....
	Render();
	Sleep(1500);

}

void ConfigMagicApp::SetupRNKP()
{
	CreateDirectory(PrepDir, NULL);
	CopyFile("D:\\default.xbe", "E:\\Prep\\default.xbe", NULL);
	CopyFile(Dummy_File, "E:\\Prep\\rnkp.bin", NULL);
}

void ConfigMagicApp::KernelFontInstall()
{
	LPSTR Install_Font_Partition = new CHAR[2048];
	ZeroMemory(Install_Font_Partition, 2048);
	CHAR tmpFileName[FILENAME_MAX];
	CHAR tmpFileName2[FILENAME_MAX];
	CHAR tmpData[256];
	LPSTR tmpFileStr = new CHAR[2048];
	LPSTR tmpFileStr2 = new CHAR[2048];
	DWORD tmpSize = 256;
	ZeroMemory(tmpData, tmpSize);
	ZeroMemory(tmpFileStr, 2048);
	ZeroMemory(tmpFileStr2, 2048);
	tmpSize = 256;
	ZeroMemory(tmpData, tmpSize);
	sprintf(tmpData, "K.%d.%d.%d.%d", *((USHORT*)XboxKrnlVersion),*((USHORT*)XboxKrnlVersion+1),*((USHORT*)XboxKrnlVersion+2),*((USHORT*)XboxKrnlVersion+3));
	strcat(tmpFileStr, tmpData);
	strcat(tmpFileStr2, "Installing " );
	strcat(tmpFileStr2, tmpData );
	strcat(tmpFileStr2, " Font" );
	ZeroMemory(tmpFileName, FILENAME_MAX);
	if (file_exist(Update_Font))
	{
		strcat(tmpFileName, Font_Path_Alt);
		Install_Font_Partition = xbox_xtf_File_Alt;
	}
	else
	{
		strcat(tmpFileName, Font_Path);
		Install_Font_Partition = xbox_xtf_File;
	}
	strcat(tmpFileName, tmpFileStr);
	ZeroMemory(tmpFileName2, FILENAME_MAX);
	strcat(tmpFileName2, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\");
	strcat(tmpFileName2, tmpFileStr);
	strcat(tmpFileName2, ".png");
	if (file_exist(tmpFileName))
	{
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText(tmpFileStr2);
		Render();
		Sleep(1000);
		remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png");
		remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.3944.1.png");
		remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4034.1.png");
		remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4627.1.png");
		remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4817.1.png");
		remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5101.1.png");
		remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5530.1.png");
		remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5713.1.png");
		remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5838.1.png");
		remove(Install_Font_Partition);
		CopyFile(tmpFileName, Install_Font_Partition, 1);
		CopyFile(EnabledPNG, tmpFileName2, 1);
		remove(Update_Font);
	}
	else
	{
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("No Compatible Font Found");
		Render();
		Sleep(1000);
		remove(Update_Font);
	}
}

void ConfigMagicApp::CreateEEPROMBackup()
{		
	//Create E:\Backups\EEPROM directory
	CreateDirectory(Backup_EEPROM_Save_Path1, NULL);
	CreateDirectory(Backup_EEPROM_Save_Path2, NULL);
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("BACKING UP XBOX EEPROM");
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
	Render();
	//save current eeprom context..
	XKEEPROM::EEPROMDATA currentEEPROM;
	m_pXKEEPROM->GetEEPROMData(&currentEEPROM);
	BOOL EncryptedState = m_pXKEEPROM->IsEncrypted();

	//Read XBOX EEPROM and save to disk..
	m_pXKEEPROM->ReadFromXBOX();
	//incase path is on these drives..
	XKUtils::MountDiskE();
	CHAR tmpFileName[FILENAME_MAX];
	
	//Create Full path for BIN File..
	ZeroMemory(tmpFileName, FILENAME_MAX);
	strcat(tmpFileName, Backup_Path);
	strcat(tmpFileName, "eeprom.bin");
	remove((LPCSTR)tmpFileName);
	m_pXKEEPROM->WriteToBINFile((LPCSTR)tmpFileName);
	
	//Write XBOX Information into .TXT file...
	ZeroMemory(tmpFileName, FILENAME_MAX);
	strcat(tmpFileName, Backup_Path);
	strcat(tmpFileName, "eeprom.cfg");
	remove((LPCSTR)tmpFileName);
	m_pXKEEPROM->WriteToCFGFile((LPCSTR)tmpFileName);

	//Create Full path for TXT File..
	ZeroMemory(tmpFileName, FILENAME_MAX);
	strcat(tmpFileName, Backup_Path);
	strcat(tmpFileName, "xbox info.txt");
	remove(tmpFileName);
	WriteTXTInfoFile(tmpFileName);

	XKUtils::UnMountDiskE();

	//switch eeprom context Back to previous
	if (EncryptedState)
	m_pXKEEPROM->SetEncryptedEEPROMData(&currentEEPROM);
	else 
	m_pXKEEPROM->SetDecryptedEEPROMData(m_XBOX_Version, &currentEEPROM);
	
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Complete");
	Render();
	Sleep(1000);
}

void ConfigMagicApp::WriteTXTInfoFile(LPCSTR strFilename)
{
	
	BOOL retVal = FALSE;
	DWORD dwBytesWrote = 0;
	CHAR tmpData[256];
	LPSTR tmpFileStr = new CHAR[2048];
	DWORD tmpSize = 256;
	ZeroMemory(tmpData, tmpSize);
	ZeroMemory(tmpFileStr, 2048);
	
	HANDLE hf = CreateFile(strFilename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hf !=  INVALID_HANDLE_VALUE)
	{
		
		//Get Xbox version
		m_EnryptedRegionValid = TRUE;
		m_XBOX_EEPROM_Current = TRUE;
		m_XBOX_Version = m_pXKEEPROM->GetXBOXVersion();

		//Re-Encrypt with correct Version...
		m_pXKEEPROM->EncryptAndCalculateCRC(m_XBOX_Version);
		
		//Time code
		time_t rawtime;
		struct tm * timeinfo;
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		
		//Write File Header..
		strcat(tmpFileStr, "*******  XBOX & EEPROM INFORMATION FILE\r\n");
		sprintf(tmpData, "*******  Backed up on: %s", asctime (timeinfo) );
		strcat(tmpFileStr, tmpData);

		if (m_XBOX_Version == XKEEPROM::V1_0)
		strcat(tmpFileStr, "\r\nXBOX Version:\t\t\tV1.0");
		if (m_XBOX_Version == XKEEPROM::V1_1)
		strcat(tmpFileStr, "\r\nXBOX Version:\t\t\tV1.1/2/3/4/5");
		if (m_XBOX_Version == XKEEPROM::V1_6) 
		strcat(tmpFileStr, "\r\nXBOX Version:\t\t\tV1.6");

		//Get Memory Status
		strcat(tmpFileStr, "\r\nXBOX RAM in MBytes:\t\t");
		tmpSize = 256;
		ZeroMemory(tmpData, tmpSize);
		MEMORYSTATUS stat;
		GlobalMemoryStatus( &stat );
		ltoa(stat.dwTotalPhys/1024/1024, tmpData, 10);
		strcat(tmpFileStr, tmpData);
		
		//Get Kernel Version
		tmpSize = 256;
		ZeroMemory(tmpData, tmpSize);
		sprintf(tmpData, "\r\nKernel Version:\t\t\t%d.%d.%d.%d", *((USHORT*)XboxKrnlVersion),*((USHORT*)XboxKrnlVersion+1),*((USHORT*)XboxKrnlVersion+2),*((USHORT*)XboxKrnlVersion+3));
		strcat(tmpFileStr, tmpData);

		//Write Serial Number..
		strcat(tmpFileStr, "\r\n\r\nXBOX Serial Number:\t\t");
		tmpSize = 256;
		ZeroMemory(tmpData, tmpSize);
		m_pXKEEPROM->GetSerialNumberString(tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//Write MAC Address..
		strcat(tmpFileStr, "\r\nXBOX MAC Address:\t\t");
		tmpSize = 256;
		ZeroMemory(tmpData, tmpSize);
		m_pXKEEPROM->GetMACAddressString(tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//Write Online Key ..
		strcat(tmpFileStr, "\r\nXBOX Online Key:\t\t");
		tmpSize = 256;
		ZeroMemory(tmpData, tmpSize);
		m_pXKEEPROM->GetOnlineKeyString(tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//Write VideoMode ..
		strcat(tmpFileStr, "\r\nXBOX Video Mode:\t\t");
		VIDEO_STANDARD vdo = m_pXKEEPROM->GetVideoStandardVal();
		if (vdo == XKEEPROM::VIDEO_STANDARD::PAL_I)
		strcat(tmpFileStr, "PAL");
		else
		strcat(tmpFileStr, "NTSC");

		//Write XBE Region..
		strcat(tmpFileStr, "\r\nXBOX XBE Region:\t\t");
		tmpSize = 256;
		ZeroMemory(tmpData, tmpSize);
		m_pXKEEPROM->GetXBERegionString(tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//Write HDDKey..
		strcat(tmpFileStr, "\r\nXBOX HDD Key:\t\t\t");
		tmpSize = 256;
		ZeroMemory(tmpData, tmpSize);
		m_pXKEEPROM->GetHDDKeyString(tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//Write Confounder..
		strcat(tmpFileStr, "\r\nXBOX Confounder:\t\t");
		tmpSize = 256;
		ZeroMemory(tmpData, tmpSize);
		m_pXKEEPROM->GetConfounderString(tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//GET HDD Info...
		//Query ATA IDENTIFY
		XKHDD::ATA_COMMAND_OBJ cmdObj;
		ZeroMemory(&cmdObj, sizeof(XKHDD::ATA_COMMAND_OBJ));
		cmdObj.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
		cmdObj.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
		XKHDD::SendATACommand(IDE_PRIMARY_PORT, &cmdObj, IDE_COMMAND_READ);
		
		//Write HDD Model
		strcat(tmpFileStr, "\r\n\r\nXBOX HDD Model:\t\t\t");
		tmpSize = 256;
		ZeroMemory(tmpData, tmpSize);
		XKHDD::GetIDEModel(cmdObj.DATA_BUFFER, (LPSTR)tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//Write HDD Serial..
		strcat(tmpFileStr, "\r\nXBOX HDD Serial:\t\t");
		tmpSize = 256;
		ZeroMemory(tmpData, tmpSize);
		XKHDD::GetIDESerial(cmdObj.DATA_BUFFER, (LPSTR)tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);
		
		//Write HDD Password..
		tmpSize = 256;
		ZeroMemory(tmpData, tmpSize);
		strcat(tmpFileStr, "\r\n\r\nXBOX HDD Password:\t\t");

		//Need decrypted HDD Key to calculate password !!
		BOOL OldState = m_pXKEEPROM->IsEncrypted();
		if (OldState)
		m_pXKEEPROM->Decrypt();
		XKEEPROM::EEPROMDATA tmptEEP;
		m_pXKEEPROM->GetEEPROMData(&tmptEEP);
		tmpSize = 256;
		ZeroMemory(tmpData, tmpSize);
		BYTE HDDpwd[20];ZeroMemory(HDDpwd, 20);
		XKHDD::GenerateHDDPwd((UCHAR*)&tmptEEP.HDDKkey, cmdObj.DATA_BUFFER, (UCHAR*)&HDDpwd);
		XKGeneral::BytesToHexStr(HDDpwd, 20, tmpData);
		strcat(tmpFileStr, tmpData);
		if (OldState)
		m_pXKEEPROM->EncryptAndCalculateCRC(m_XBOX_Version);

		//Query ATAPI IDENTIFY
		ZeroMemory(&cmdObj, sizeof(XKHDD::ATA_COMMAND_OBJ));
		cmdObj.IPReg.bCommandReg = IDE_ATAPI_IDENTIFY;
		cmdObj.IPReg.bDriveHeadReg = IDE_DEVICE_SLAVE;
		XKHDD::SendATACommand(IDE_PRIMARY_PORT, &cmdObj, IDE_COMMAND_READ);
		
		//Write DVD Model
		strcat(tmpFileStr, "\r\n\r\nXBOX DVD Model:\t\t\t");
		tmpSize = 256;
		ZeroMemory(tmpData, tmpSize);
		XKHDD::GetIDEModel(cmdObj.DATA_BUFFER, (LPSTR)tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);
		strupr(tmpFileStr);
		WriteFile(hf, tmpFileStr, (DWORD)strlen(tmpFileStr), &dwBytesWrote, NULL);
	}
	delete[] tmpFileStr;
	CloseHandle(hf);
}

void ConfigMagicApp::UnLock_New_HDD()
{	
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("UNLOCKING HARD DRIVE");
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
	Render();
	m_pXKEEPROM->ReadFromXBOX();
	m_XBOX_EEPROM_Current = TRUE;
	//ATA Command Structure..
	XKHDD::ATA_COMMAND_OBJ hddcommand;
	UCHAR HddPass[32];
	//Dont Lock if the EEPROM data was NOT read from XBOX
	if(m_XBOX_EEPROM_Current)
	{
		//Decrypting EEPROM, if it fails.. Display fail message!
		if (m_pXKEEPROM->Decrypt())
		{
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Checking HDD");
			Render();
			//Get IDE_ATA_IDENTIFY Data for HDD ..
			ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
			hddcommand.DATA_BUFFSIZE = 0;
			hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
			hddcommand.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
			XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_READ);

			XKEEPROM::EEPROMDATA tmpData;
			m_pXKEEPROM->GetEEPROMData(&tmpData);
			XKHDD::GenerateHDDPwd(tmpData.HDDKkey, hddcommand.DATA_BUFFER, HddPass);
			
			//Get ATA Locked State
			DWORD SecStatus = XKHDD::GetIDESecurityStatus(hddcommand.DATA_BUFFER);
			//Check if Disk is Reall locked..
			if ((SecStatus & IDE_SECURITY_SUPPORTED) && (SecStatus & IDE_SECURITY_ENABLED))
			{
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Unlocking HDD");
				Render();
				//Execute HDD Unlock..
				ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
				hddcommand.DATA_BUFFSIZE = 512;
				hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
				hddcommand.IPReg.bCommandReg = IDE_ATA_SECURITY_UNLOCK;
				LPBYTE HDDP = (LPBYTE)&hddcommand.DATA_BUFFER;
				memcpy(HDDP+2, HddPass, 20);
				XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_WRITE);

				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Checking HDD");
				Render();
				//Query HDD To see if we succeeded..
				ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
				hddcommand.DATA_BUFFSIZE = 512;
				hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
				hddcommand.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
				XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_READ);

				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Disable HDD Password");
				Render();
				//Execute HDD Lock Disable
				ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
				hddcommand.DATA_BUFFSIZE = 512;
				hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
				hddcommand.IPReg.bCommandReg = IDE_ATA_SECURITY_DISABLE;
				memcpy(HDDP+2, HddPass, 20);
				XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_WRITE);

				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Checking HDD");
				Render();

				//We Found That sometimes that by the first query it hasnt unlocked yet.. so we query three times if needed..
				int retrycnt = 0;
				do
				{
					//Query HDD To see if we succeeded..
					ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
					hddcommand.DATA_BUFFSIZE = 512;
					hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
					hddcommand.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
					XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_READ);
					SecStatus = XKHDD::GetIDESecurityStatus(hddcommand.DATA_BUFFER);
					
					retrycnt++;
					
				} while (!((SecStatus & IDE_SECURITY_SUPPORTED) && !(SecStatus & IDE_SECURITY_ENABLED)) && (retrycnt < 3));

				if ((SecStatus & IDE_SECURITY_SUPPORTED) && !(SecStatus & IDE_SECURITY_ENABLED))
				{
					((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Xbox HDD is now unlocked");
					Render();
					Sleep(1500);
				}
				else
				{
					((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("FAILED TO UNLOCK XBOX HDD !!");
					Render();
					LED_Flash_Red_Fast;
					Sleep(5000);
					XKUtils::LaunchXBE(NKPatcherSettings);
				}				
			}
			else
			{
				((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("XBOX HDD IS NOT LOCKED ??");
				Render();
				LED_Flash_Red_Fast;
				Sleep(3000);
			}
			//Re Encrypt EEPROM with Current XBOX Version
			m_pXKEEPROM->EncryptAndCalculateCRC(m_XBOX_Version);
		}
		else
		{
			//Encrypted Region is Invalid.. DONT Allow Locking/Unlocking..
			m_EnryptedRegionValid = FALSE;
			((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("EEPROM ENCRYPTED REGION INVALID");
			Render();
			LED_Flash_Red_Fast;
			Sleep(5000);
			XKUtils::LaunchXBE(NKPatcherSettings);
		}
	}
}

void ConfigMagicApp::Write_XBOX_EEPROM()
{
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("UPDATING EEPROM");
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
	Render();
	//ATA Command Structure..
	XKHDD::ATA_COMMAND_OBJ hddcommand;
	//Decrypting EEPROM, if it fails.. Display fail message!
	if (m_pXKEEPROM->Decrypt())
	{
		//Re-Encrypt with correct Version...
		m_EnryptedRegionValid = TRUE;
		m_XBOX_Version = m_pXKEEPROM->GetXBOXVersion();
		m_pXKEEPROM->EncryptAndCalculateCRC(m_XBOX_Version);
		
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Checking HDD");
		Render();
		//Get IDE_ATA_IDENTIFY Data for HDD ..
		ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
		hddcommand.DATA_BUFFSIZE = 0;
		hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
		hddcommand.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
		XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_READ);

		//Get ATA Locked State
		DWORD SecStatus = XKHDD::GetIDESecurityStatus(hddcommand.DATA_BUFFER);
		//Check if Disk is locked..
		if (!(SecStatus & IDE_SECURITY_ENABLED))
		{
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Writing EEPROM Data to XBOX");
			Render();
			if (m_pXKEEPROM->IsEncrypted())
			{
				m_pXKEEPROM->WriteToXBOX();
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("EEPROM Has Been Updated");
				Render();
				Sleep(1000);
			}
			else
			{
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatusMsg"))->SetText("EEPROM NOT ENCRYPTED");
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("EEPROM Has Not Been Updated");
				Render();
				LED_Flash_Red_Fast;
				Sleep(5000);
			}
		}
		else
		{
			((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("UNLOCK FAILED TO UNLOCK");
			Render();
			LED_Flash_Red_Orange;
			Sleep(5000);
			XKUtils::LaunchXBE(NKPatcherSettings);
		}
	}
	else
	{
		//Encrypted Region is Invalid.. DONT Allow EEPROM Update..
		m_EnryptedRegionValid = FALSE;
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("EEPROM ENCRYPTED REGION INVALID !!");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("EEPROM Has Not Been Updated");
		Render();
		LED_Flash_Red;
		Sleep(5000);
	}
}

void ConfigMagicApp::Lock_New_HDD()
{
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("LOCKING XBOX HDD");
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
	Render();
	m_pXKEEPROM->ReadFromXBOX();
	m_XBOX_EEPROM_Current = TRUE;
	//ATA Command Structure..
	XKHDD::ATA_COMMAND_OBJ hddcommand;
	UCHAR HddPass[32];
	UCHAR MasterPassword[13] = "TEAMASSEMBLY";
	
	//Dont Lock if the EEPROM data was NOT read from XBOX
	if(m_XBOX_EEPROM_Current)
	{
		//Decrypting EEPROM, if it fails.. Display fail message!
		if (m_pXKEEPROM->Decrypt())
		{

			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Checking HDD");
			Render();
			//Get IDE_ATA_IDENTIFY Data for HDD ..
			ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
			hddcommand.DATA_BUFFSIZE = 0;
			hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
			hddcommand.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
			XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_READ);

			XKEEPROM::EEPROMDATA tmpData;
			m_pXKEEPROM->GetEEPROMData(&tmpData);
			XKHDD::GenerateHDDPwd(tmpData.HDDKkey, hddcommand.DATA_BUFFER, HddPass);

			//Get ATA Locked State
			DWORD SecStatus = XKHDD::GetIDESecurityStatus(hddcommand.DATA_BUFFER);
			//Check if Disk is already locked..
			if ((SecStatus & IDE_SECURITY_SUPPORTED) && !(SecStatus & IDE_SECURITY_ENABLED))
			{

				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Setting Master Password");
				Render();
				//Execute HDD Unlock..
				ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
				hddcommand.DATA_BUFFSIZE = 512;
				hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
				hddcommand.IPReg.bCommandReg = IDE_ATA_SECURITY_SETPASSWORD;
				LPBYTE HDDP = (LPBYTE)&hddcommand.DATA_BUFFER;
				LPDWORD pMastr = (LPDWORD) HDDP;
				*pMastr = 0x0001; //Set Master Pwd..
				memcpy(HDDP+2, MasterPassword, 13);
				XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_WRITE);
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Master Password is TEAMASSEMBLY");
				Render();
				Sleep(2000);
				
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Checking HDD");
				Render();
				//Query HDD To see if we succeeded..
				ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
				hddcommand.DATA_BUFFSIZE = 512;
				hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
				hddcommand.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
				XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_READ);
				SecStatus = XKHDD::GetIDESecurityStatus(hddcommand.DATA_BUFFER);

				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Locking HDD");
				Render();
				//Execute HDD Unlock..
				ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
				hddcommand.DATA_BUFFSIZE = 512;
				hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
				hddcommand.IPReg.bCommandReg = IDE_ATA_SECURITY_SETPASSWORD;
				memcpy(HDDP+2, HddPass, 20);
				XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_WRITE);

				//We Found That sometimes that by the first query it hasnt locked yet.. so we query twice if needed..
				int retrycnt = 0;
				do
				{
					((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Checking HDD");
					Render();
					//Query HDD To see if we succeeded..
					ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
					hddcommand.DATA_BUFFSIZE = 512;
					hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
					hddcommand.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
					XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_READ);
					SecStatus = XKHDD::GetIDESecurityStatus(hddcommand.DATA_BUFFER);

					retrycnt++;

				} while (!((SecStatus & IDE_SECURITY_SUPPORTED) && (SecStatus & IDE_SECURITY_ENABLED)) && (retrycnt < 3));

				if ((SecStatus & IDE_SECURITY_SUPPORTED) && (SecStatus & IDE_SECURITY_ENABLED))
				{
					((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("HDD locked successfully");
					Render();
					Sleep(1000);
				}
				else
				{
					((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("FAILED TO LOCK XBOX HDD !!");
					Render();
					LED_Flash_Red_Fast;
					Sleep(5000);
					XKUtils::LaunchXBE(NKPatcherSettings);
				}
			}
			else
			{
				if (!(SecStatus & IDE_SECURITY_SUPPORTED))
				((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("HDD DOES NOT SUPPORT LOCKING !");
				else
				((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("XBOX HDD IS ALREADY LOCKED ??");

				Render();
				LED_Flash_Red_Fast;
				Sleep(5000);
				XKUtils::LaunchXBE(NKPatcherSettings);
			}
			// Re-Encrypt EEPROM with current XBOX Version..
			m_pXKEEPROM->EncryptAndCalculateCRC(m_XBOX_Version);
		}
		else
		{
			//Encrypted Region is Invalid.. DONT Allow Locking/Unlocking..
			m_EnryptedRegionValid = FALSE;
			((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("EEPROM ENCRYPTED REGION INVALID");
			Render();
			LED_Flash_Red_Fast;
			Sleep(5000);
			XKUtils::LaunchXBE(NKPatcherSettings);
		}
	}
	else
	{
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("LOAD XBOX EEPROM TO LOCK !!");
		Render();
		LED_Flash_Red_Fast;
		Sleep(5000);
		XKUtils::LaunchXBE(NKPatcherSettings);
	}
}

void ConfigMagicApp::Force_Write_XBOX_EEPROM()
{
	//Decrypting EEPROM, if it fails.. Display fail message!
	if (m_pXKEEPROM->Decrypt())
	{
		//Re-Encrypt with correct Version...
		m_EnryptedRegionValid = TRUE;
		m_XBOX_Version = m_pXKEEPROM->GetXBOXVersion();
		m_pXKEEPROM->EncryptAndCalculateCRC(m_XBOX_Version);
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Writing EEPROM Data to XBOX");
		Render();
		if (m_pXKEEPROM->IsEncrypted())
		{
			m_pXKEEPROM->WriteToXBOX();
			LED_Flash_Orange;
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("EEPROM Has Been Updated");
			Render();
			LED_Orange;
			Sleep(1000);
		}
		else
		{
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatusMsg"))->SetText("EEPROM NOT ENCRYPTED");
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("EEPROM Has Not Been Updated");
			Render();
			LED_Flash_Red_Fast;
			Sleep(5000);
		}
	}
	else
	{
		//Encrypted Region is Invalid.. DONT Allow EEPROM Update..
		m_EnryptedRegionValid = FALSE;
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("EEPROM ENCRYPTED REGION INVALID !!");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("EEPROM Has Not Been Updated");
		Render();
		LED_Flash_Red;
		Sleep(5000);
	}
}

void ConfigMagicApp::CheckBios()
{
	if (file_exist(Backup_Bios))
	{
		remove(Backup_Bios);
		BYTE data;
		char *BIOS_Name;
		CStdString strBiosName;
		DWORD addr        = FLASH_BASE_ADDRESS;
		DWORD addr_kernel = KERNEL_BASE_ADDRESS;
		CXBoxFlash mbFlash;
		char * flash_copy;

		flash_copy = (char *) malloc(0x100000);

		BIOS_Name     = (char*) malloc(100);

		struct Bios *Listone = LoadBiosSigns();

		if( !Listone )
		{
			free(BIOS_Name);
		}

		for(int loop=0;loop<0x100000;loop++)
		{
			data = mbFlash.Read(addr++);
			flash_copy[loop] = data;
		}

		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("CHECKING BIOS");
		Render();
		Sleep(2000);
		// Detect a 1024 KB Bios MD5
		MD5Buffer (flash_copy,0,1024);
		strcpy(BIOS_Name,CheckMD5(Listone, MD5_Sign));
		if ( strcmp(BIOS_Name, "Unknown") == 0)
		{ 
			bios_dumped = 0;
		}
		else
		{		
			bios_dumped = 1;
		}
		strBiosName = BIOS_Name;
		if ( (bios_dumped == 1) && (strBiosName == "Retail 3944") || (strBiosName == "Retail 4034") || (strBiosName == "Retail 4134") || (strBiosName == "Retail 4817") || (strBiosName == "Retail 5101") || (strBiosName == "Retail 5530") || (strBiosName == "Retail 5713") || (strBiosName == "Retail 5838") )
		{
			((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText(strBiosName+" BIOS DETECTED");
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Backing Up BIOS");
			Render();
			Sleep(2000);
			CreateDirectory("E:\\Backups", NULL);
			CreateDirectory("E:\\Backups\\BIOS", NULL);
			FILE *fp;
			DWORD addr        = FLASH_BASE_ADDRESS;
			DWORD addr_kernel = KERNEL_BASE_ADDRESS;
			char * flash_copy, data;
			CXBoxFlash mbFlash;

			flash_copy = (char *) malloc(0x100000);
			
			if((fp = fopen(Retail_Bios_Save_Path+strBiosName+" Bios.bin", "wb")) != NULL)
			{
				for(int loop=0;loop<0x100000;loop++)
				{
					data = mbFlash.Read(addr++);
					flash_copy[loop] = data;
				}
				if ( (MD5Buffer(flash_copy,0,256) == MD5Buffer(flash_copy,262144,256)) && (MD5Buffer(flash_copy,524288,256)== MD5Buffer(flash_copy,786432,256)) )
				{
					fwrite(flash_copy, 0x40000, 1, fp);
					fclose(fp);
				}
			}
			else
			{
				if ((MD5Buffer(flash_copy,0,512)) == (MD5Buffer(flash_copy,524288,512)))
				{
					fwrite(flash_copy, 0x80000, 1, fp);
					fclose(fp);
				}
				else
				{
					if((fp = fopen(Retail_Bios_Save_Path+strBiosName+" Bios.bin", "wb")) != NULL)
					{
						fwrite(flash_copy, 0x100000, 1, fp);
						fclose(fp);
					}
				}
			}
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Complete");
			Render();
			Sleep(1500);
		}
		XKUtils::LaunchXBE(NKPatcherSettings);
	}
}

struct Bios * ConfigMagicApp::LoadBiosSigns()
{
	FILE *infile;

	if ((infile = fopen(Retail_Bios_Hash_File,"r")) == NULL)
	{
		return NULL;
	}
	else
	{
		struct Bios * Listone = (struct Bios *)calloc(1000, sizeof(struct Bios));
		int cntBioses=0;
		char buffer[255];
		char stringone[255];
		do
		{
			fgets(stringone,255,infile);
			if  (stringone[0] != '#')
			{
				if (strstr(stringone,"=")!= NULL)
				{
					strcpy(Listone[cntBioses].Name,ReturnBiosName(buffer, stringone));
					strcpy(Listone[cntBioses].Signature,ReturnBiosSign(buffer, stringone));
					cntBioses++;
				}
			}
		} while( !feof( infile ) && cntBioses < 999 );
		fclose(infile);
		strcpy(Listone[cntBioses++].Name,"\0");
		strcpy(Listone[cntBioses++].Signature,"\0");
		return Listone;
	}
}

char* ConfigMagicApp::MD5Buffer(char *buffer, long PosizioneInizio,int KBytes)
{
	XBMC::XBMC_MD5 mdContext;
	CStdString md5sumstring;
	mdContext.append((unsigned char *)(buffer + PosizioneInizio), KBytes * 1024);
	mdContext.getDigest(md5sumstring);
	strcpy(MD5_Sign, md5sumstring.c_str());
	return MD5_Sign;
}

char* ConfigMagicApp::ReturnBiosName(char *buffer, char *str)
{
	int cnt1,cnt2,i;
	cnt1=cnt2=0;

	for (i=0;i<255;i++) buffer[i]='\0';

	while (str[cnt2] != '=')
	{
		buffer[cnt1]=str[cnt2];
		cnt1++;
		cnt2++;
	}
	buffer[cnt1++]='\0';
	return buffer;
}
char* ConfigMagicApp::ReturnBiosSign(char *buffer, char *str)
{
	int cnt1,cnt2,i;
	cnt1=cnt2=0;
	for (i=0;i<255;i++) buffer[i]='\0';
	while (str[cnt2] != '=') cnt2++;
	cnt2++;
	while (str[cnt2] != NULL)
	{
		if ( str[cnt2] != ' ' )
		{
			buffer[cnt1]=toupper(str[cnt2]);
			cnt1++;
			cnt2++;
		}
		else cnt2++;
	}
	buffer[cnt1++]='\0';
	return buffer;
}
char* ConfigMagicApp::CheckMD5 (struct Bios *Listone, char *Sign)
{
	int cntBioses;
	cntBioses=0;
	do
	{
		if  (strstr(Listone[cntBioses].Signature, Sign) != NULL)
		{ return (Listone[cntBioses].Name);   }
		cntBioses++;
	}
	while( strcmp(Listone[cntBioses].Name,"\0") != 0);
	return ("Unknown");
}

int ConfigMagicApp::file_exist(const char *name)
{
  struct stat   buffer;
  return (stat (name, &buffer) == 0);
}

HRESULT ConfigMagicApp::Initialize()
{
	HRESULT retVal = S_OK;
	m_pXKEEPROM = new XKEEPROM();
	m_EnryptedRegionValid = FALSE;
	m_XBOX_EEPROM_Current = FALSE;
	//incase path is on these drives..
	XKUtils::MountDiskC();
	XKUtils::MountDiskE();
	XKUtils::MountDiskS();
	// Check for font files
	if (file_exist("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\Xbox softmod tool kit\\Media\\Font_Message.xpr")) // Size 24
	{
		retVal = m_XBFont1.Create(m_pd3dDevice, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\Xbox softmod tool kit\\Media\\Font_Message.xpr");
	}
	else
	{
		retVal = m_XBFont1.Create(m_pd3dDevice, "D:\\Media\\Font_Message.xpr");
	}
	if (file_exist("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\Xbox softmod tool kit\\Media\\Font_Status.xpr")) // Size 20
	{
		retVal = m_XBFont2.Create(m_pd3dDevice, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\Xbox softmod tool kit\\Media\\Font_Status.xpr");
	}
	else
	{
		retVal = m_XBFont2.Create(m_pd3dDevice, "D:\\Media\\Font_Status.xpr");
	}
	//Load Settings from Config File.. 
	LoadSettings();
	//Initialise the display.
	m_MenuXOffset = 0;
	m_MenuYOffset = 24;
	ScreenWidth = 640 - XImgCorrection;
	ScreenHeight = 480 - YImgCorrection;
	D3DXCreateSprite(m_pd3dDevice, &m_bgSprite);
	InitScreen();
	m_ActiveForm = m_pFrmStatus;
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("INITIALISING");
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
	if (file_exist(RNKP_File))
	{
		Sleep(0);
	}
	else
	{
		Render();
		Sleep(2000);
	}
	// Factory reset
	NTSCResetToFactoryEEPROM();
	PALResetToFactoryEEPROM();
	// Create dummy_file
	std::ofstream CreateDummy(Dummy_File, std::ios::binary | std::ios::out);
	CreateDummy.write("", 0);
	CreateDummy.close();
	//Functions ( These check for specific files )
	SetLanguage();
	VirtualEEPROMRemoval();
	EnableVirtualEEPROM();
	SetUpDashboard();
	Stage2OfInstallSoftmod();
	Stage2OfUpdateSoftmod();
	UpdateKernelFont();
	RestoreKernelFont();
	ReloadNKPatcherSettings();
	NullHardDriveKey();
	LockHardDrive();
	ChangeVideoRegionToNTSC();
	ChangeVideoRegionToPAL();
	NULLEEPROMCheckSum3();
	PatchNKPatcherToSupportOtherPartitions();
	PatchNKPatcherToSupportDualBoot();
	DumpDecryptedEEPROM();
	EnablePersistentSoftmodState();
	DisablePersistentSoftmodState();
	CheckBios();
	RemoveParentalControls();
	//Default mode
	Sleep(100);
	LED_Flash_Green_Orange;
	CreateEEPROMBackup();
	LED_Orange;
	XKUtils::LaunchXBE(NKPatcherSettings);
	return retVal;
}

void ConfigMagicApp::InitScreen()
{
	m_pFrmStatus = new XKControl_Panel("Status", m_bgSprite, XOffset, YOffset, ScreenWidth,ScreenHeight);
	// Check for image file
	if (file_exist("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\Xbox softmod tool kit\\Media\\background.png"))
	{
		m_pFrmStatus->LoadImage("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\Xbox softmod tool kit\\Media\\background.png");
	}
	else
	{	
		m_pFrmStatus->LoadImage("D:\\Media\\background.png");
	}
	txtStatusMsg = new XKControl_TextBox("txtStatusMsg", m_bgSprite);
	txtStatusMsg->SetBitmapFont(&m_XBFont1);
	txtStatusMsg->SetTextColor(m_clrMessageTextColor);
	txtStatusMsg->SetTextCentered(TRUE);
	txtStatusMsg->SetTextShadow(FALSE);
	txtStatus = new XKControl_TextBox("txtStatus", m_bgSprite);
	txtStatus->SetBitmapFont(&m_XBFont2);
	txtStatus->SetTextColor(m_clrStatusTextColor);
	txtStatus->SetTextCentered(TRUE);
	txtStatus->SetTextShadow(FALSE);
	m_pFrmStatus->AddControl(txtStatusMsg,  0, 180,  ScreenWidth, 30);
	m_pFrmStatus->AddControl(txtStatus,		0, 400,  ScreenWidth, 30);
}

void ConfigMagicApp::ResetLocations(LONG xoffset, LONG yoffset)
{
	XOffset = (xoffset<=0)?0:xoffset;
	YOffset = (yoffset<=0)?0:yoffset;
	m_pFrmInfo->SetPosition(XOffset, YOffset);
	m_pFrmStatus->SetPosition(XOffset, YOffset);
	txtStatus->SetPosition(XOffset, YOffset+405);
}

HRESULT ConfigMagicApp::Render()
{
	//Clear screen..
	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE); //some reason rendergradientbg doesn't wrok if this is false...
	RenderGradientBackground(0, 0);
	//Render Background Image..
	if (m_ActiveForm->IsImageLoaded())
	m_ActiveForm->Render();
	//Present Data
	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	return S_OK;
}

void ConfigMagicApp::LoadSettings()
{
	//*************************** READ SCREEN SIZE, OFFSETS & COLOURS *****************************************
	XOffset = 25;
	YOffset = 20;
	XImgCorrection = 45;
	YImgCorrection = 40;
	m_clrMessageTextColor		= 0xffefcb49;
	m_clrStatusTextColor		= 0xff979797;
}

//Application start point
void main()
{
	LED_Orange;
	ConfigMagicApp xbapp;
	xbapp.Create();
	xbapp.SetAppTitle("Xbox Softmod Tool Kit");
	xbapp.Run();
}