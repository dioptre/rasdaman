// This query tests different selectivities with moving query boxes
// on tomo_cubed with 64kB tiles ([0:39,0:39,0:39]). Each selectivity
// is repeated 20 times with query boxes of the same shape and size
// starting at different origins.

// [0.5]: [205:249, 154:198, 39:65]
SELECT img[205:249, 154:198, 39:65]
FROM tomo_cubed_64 AS img
// [0.5]: [125:169, 124:168, 15:41]
SELECT img[125:169, 124:168, 15:41]
FROM tomo_cubed_64 AS img
// [0.5]: [197:241, 71:115, 82:108]
SELECT img[197:241, 71:115, 82:108]
FROM tomo_cubed_64 AS img
// [0.5]: [53:97, 192:236, 122:148]
SELECT img[53:97, 192:236, 122:148]
FROM tomo_cubed_64 AS img
// [0.5]: [140:184, 93:137, 121:147]
SELECT img[140:184, 93:137, 121:147]
FROM tomo_cubed_64 AS img
// [0.5]: [154:198, 82:126, 120:146]
SELECT img[154:198, 82:126, 120:146]
FROM tomo_cubed_64 AS img
// [0.5]: [209:253, 51:95, 47:73]
SELECT img[209:253, 51:95, 47:73]
FROM tomo_cubed_64 AS img
// [0.5]: [57:101, 159:203, 113:139]
SELECT img[57:101, 159:203, 113:139]
FROM tomo_cubed_64 AS img
// [0.5]: [98:142, 166:210, 12:38]
SELECT img[98:142, 166:210, 12:38]
FROM tomo_cubed_64 AS img
// [0.5]: [194:238, 88:132, 52:78]
SELECT img[194:238, 88:132, 52:78]
FROM tomo_cubed_64 AS img
// [0.5]: [162:206, 101:145, 53:79]
SELECT img[162:206, 101:145, 53:79]
FROM tomo_cubed_64 AS img
// [0.5]: [166:210, 60:104, 126:152]
SELECT img[166:210, 60:104, 126:152]
FROM tomo_cubed_64 AS img
// [0.5]: [127:171, 157:201, 92:118]
SELECT img[127:171, 157:201, 92:118]
FROM tomo_cubed_64 AS img
// [0.5]: [46:90, 94:138, 53:79]
SELECT img[46:90, 94:138, 53:79]
FROM tomo_cubed_64 AS img
// [0.5]: [19:63, 42:86, 17:43]
SELECT img[19:63, 42:86, 17:43]
FROM tomo_cubed_64 AS img
// [0.5]: [182:226, 0:44, 97:123]
SELECT img[182:226, 0:44, 97:123]
FROM tomo_cubed_64 AS img
// [0.5]: [139:183, 98:142, 47:73]
SELECT img[139:183, 98:142, 47:73]
FROM tomo_cubed_64 AS img
// [0.5]: [106:150, 182:226, 11:37]
SELECT img[106:150, 182:226, 11:37]
FROM tomo_cubed_64 AS img
// [0.5]: [193:237, 200:244, 23:49]
SELECT img[193:237, 200:244, 23:49]
FROM tomo_cubed_64 AS img
// [0.5]: [40:84, 2:46, 5:31]
SELECT img[40:84, 2:46, 5:31]
FROM tomo_cubed_64 AS img
// [1]: [51:106, 169:224, 105:138]
SELECT img[51:106, 169:224, 105:138]
FROM tomo_cubed_64 AS img
// [1]: [72:127, 30:85, 113:146]
SELECT img[72:127, 30:85, 113:146]
FROM tomo_cubed_64 AS img
// [1]: [104:159, 82:137, 48:81]
SELECT img[104:159, 82:137, 48:81]
FROM tomo_cubed_64 AS img
// [1]: [91:146, 86:141, 40:73]
SELECT img[91:146, 86:141, 40:73]
FROM tomo_cubed_64 AS img
// [1]: [121:176, 19:74, 59:92]
SELECT img[121:176, 19:74, 59:92]
FROM tomo_cubed_64 AS img
// [1]: [69:124, 156:211, 27:60]
SELECT img[69:124, 156:211, 27:60]
FROM tomo_cubed_64 AS img
// [1]: [24:79, 64:119, 25:58]
SELECT img[24:79, 64:119, 25:58]
FROM tomo_cubed_64 AS img
// [1]: [70:125, 193:248, 90:123]
SELECT img[70:125, 193:248, 90:123]
FROM tomo_cubed_64 AS img
// [1]: [193:248, 1:56, 38:71]
SELECT img[193:248, 1:56, 38:71]
FROM tomo_cubed_64 AS img
// [1]: [118:173, 178:233, 56:89]
SELECT img[118:173, 178:233, 56:89]
FROM tomo_cubed_64 AS img
// [1]: [118:173, 23:78, 21:54]
SELECT img[118:173, 23:78, 21:54]
FROM tomo_cubed_64 AS img
// [1]: [9:64, 166:221, 44:77]
SELECT img[9:64, 166:221, 44:77]
FROM tomo_cubed_64 AS img
// [1]: [66:121, 65:120, 93:126]
SELECT img[66:121, 65:120, 93:126]
FROM tomo_cubed_64 AS img
// [1]: [63:118, 141:196, 31:64]
SELECT img[63:118, 141:196, 31:64]
FROM tomo_cubed_64 AS img
// [1]: [89:144, 58:113, 39:72]
SELECT img[89:144, 58:113, 39:72]
FROM tomo_cubed_64 AS img
// [1]: [72:127, 124:179, 93:126]
SELECT img[72:127, 124:179, 93:126]
FROM tomo_cubed_64 AS img
// [1]: [46:101, 178:233, 41:74]
SELECT img[46:101, 178:233, 41:74]
FROM tomo_cubed_64 AS img
// [1]: [52:107, 10:65, 49:82]
SELECT img[52:107, 10:65, 49:82]
FROM tomo_cubed_64 AS img
// [1]: [151:206, 5:60, 1:34]
SELECT img[151:206, 5:60, 1:34]
FROM tomo_cubed_64 AS img
// [1]: [95:150, 71:126, 85:118]
SELECT img[95:150, 71:126, 85:118]
FROM tomo_cubed_64 AS img
// [2]: [31:100, 166:235, 7:49]
SELECT img[31:100, 166:235, 7:49]
FROM tomo_cubed_64 AS img
// [2]: [109:178, 174:243, 31:73]
SELECT img[109:178, 174:243, 31:73]
FROM tomo_cubed_64 AS img
// [2]: [24:93, 32:101, 91:133]
SELECT img[24:93, 32:101, 91:133]
FROM tomo_cubed_64 AS img
// [2]: [76:145, 52:121, 33:75]
SELECT img[76:145, 52:121, 33:75]
FROM tomo_cubed_64 AS img
// [2]: [82:151, 5:74, 17:59]
SELECT img[82:151, 5:74, 17:59]
FROM tomo_cubed_64 AS img
// [2]: [48:117, 100:169, 72:114]
SELECT img[48:117, 100:169, 72:114]
FROM tomo_cubed_64 AS img
// [2]: [81:150, 183:252, 39:81]
SELECT img[81:150, 183:252, 39:81]
FROM tomo_cubed_64 AS img
// [2]: [8:77, 122:191, 5:47]
SELECT img[8:77, 122:191, 5:47]
FROM tomo_cubed_64 AS img
// [2]: [170:239, 145:214, 23:65]
SELECT img[170:239, 145:214, 23:65]
FROM tomo_cubed_64 AS img
// [2]: [84:153, 101:170, 86:128]
SELECT img[84:153, 101:170, 86:128]
FROM tomo_cubed_64 AS img
// [2]: [72:141, 147:216, 32:74]
SELECT img[72:141, 147:216, 32:74]
FROM tomo_cubed_64 AS img
// [2]: [12:81, 25:94, 108:150]
SELECT img[12:81, 25:94, 108:150]
FROM tomo_cubed_64 AS img
// [2]: [94:163, 125:194, 29:71]
SELECT img[94:163, 125:194, 29:71]
FROM tomo_cubed_64 AS img
// [2]: [4:73, 44:113, 0:42]
SELECT img[4:73, 44:113, 0:42]
FROM tomo_cubed_64 AS img
// [2]: [134:203, 7:76, 85:127]
SELECT img[134:203, 7:76, 85:127]
FROM tomo_cubed_64 AS img
// [2]: [84:153, 168:237, 3:45]
SELECT img[84:153, 168:237, 3:45]
FROM tomo_cubed_64 AS img
// [2]: [166:235, 140:209, 41:83]
SELECT img[166:235, 140:209, 41:83]
FROM tomo_cubed_64 AS img
// [2]: [98:167, 43:112, 81:123]
SELECT img[98:167, 43:112, 81:123]
FROM tomo_cubed_64 AS img
// [2]: [5:74, 137:206, 29:71]
SELECT img[5:74, 137:206, 29:71]
FROM tomo_cubed_64 AS img
// [2]: [177:246, 45:114, 28:70]
SELECT img[177:246, 45:114, 28:70]
FROM tomo_cubed_64 AS img
// [5]: [52:146, 5:99, 72:128]
SELECT img[52:146, 5:99, 72:128]
FROM tomo_cubed_64 AS img
// [5]: [147:241, 45:139, 90:146]
SELECT img[147:241, 45:139, 90:146]
FROM tomo_cubed_64 AS img
// [5]: [156:250, 145:239, 7:63]
SELECT img[156:250, 145:239, 7:63]
FROM tomo_cubed_64 AS img
// [5]: [30:124, 108:202, 74:130]
SELECT img[30:124, 108:202, 74:130]
FROM tomo_cubed_64 AS img
// [5]: [140:234, 44:138, 25:81]
SELECT img[140:234, 44:138, 25:81]
FROM tomo_cubed_64 AS img
// [5]: [48:142, 9:103, 8:64]
SELECT img[48:142, 9:103, 8:64]
FROM tomo_cubed_64 AS img
// [5]: [0:94, 157:251, 73:129]
SELECT img[0:94, 157:251, 73:129]
FROM tomo_cubed_64 AS img
// [5]: [125:219, 53:147, 7:63]
SELECT img[125:219, 53:147, 7:63]
FROM tomo_cubed_64 AS img
// [5]: [26:120, 133:227, 38:94]
SELECT img[26:120, 133:227, 38:94]
FROM tomo_cubed_64 AS img
// [5]: [157:251, 41:135, 74:130]
SELECT img[157:251, 41:135, 74:130]
FROM tomo_cubed_64 AS img
// [5]: [41:135, 120:214, 31:87]
SELECT img[41:135, 120:214, 31:87]
FROM tomo_cubed_64 AS img
// [5]: [33:127, 116:210, 71:127]
SELECT img[33:127, 116:210, 71:127]
FROM tomo_cubed_64 AS img
// [5]: [138:232, 127:221, 20:76]
SELECT img[138:232, 127:221, 20:76]
FROM tomo_cubed_64 AS img
// [5]: [13:107, 9:103, 60:116]
SELECT img[13:107, 9:103, 60:116]
FROM tomo_cubed_64 AS img
// [5]: [124:218, 139:233, 74:130]
SELECT img[124:218, 139:233, 74:130]
FROM tomo_cubed_64 AS img
// [5]: [117:211, 86:180, 91:147]
SELECT img[117:211, 86:180, 91:147]
FROM tomo_cubed_64 AS img
// [5]: [143:237, 52:146, 71:127]
SELECT img[143:237, 52:146, 71:127]
FROM tomo_cubed_64 AS img
// [5]: [10:104, 35:129, 19:75]
SELECT img[10:104, 35:129, 19:75]
FROM tomo_cubed_64 AS img
// [5]: [110:204, 64:158, 64:120]
SELECT img[110:204, 64:158, 64:120]
FROM tomo_cubed_64 AS img
// [5]: [12:106, 103:197, 96:152]
SELECT img[12:106, 103:197, 96:152]
FROM tomo_cubed_64 AS img
// [10]: [108:226, 53:171, 9:80]
SELECT img[108:226, 53:171, 9:80]
FROM tomo_cubed_64 AS img
// [10]: [46:164, 103:221, 10:81]
SELECT img[46:164, 103:221, 10:81]
FROM tomo_cubed_64 AS img
// [10]: [80:198, 73:191, 38:109]
SELECT img[80:198, 73:191, 38:109]
FROM tomo_cubed_64 AS img
// [10]: [38:156, 132:250, 26:97]
SELECT img[38:156, 132:250, 26:97]
FROM tomo_cubed_64 AS img
// [10]: [11:129, 86:204, 77:148]
SELECT img[11:129, 86:204, 77:148]
FROM tomo_cubed_64 AS img
// [10]: [46:164, 90:208, 17:88]
SELECT img[46:164, 90:208, 17:88]
FROM tomo_cubed_64 AS img
// [10]: [86:204, 125:243, 6:77]
SELECT img[86:204, 125:243, 6:77]
FROM tomo_cubed_64 AS img
// [10]: [54:172, 49:167, 69:140]
SELECT img[54:172, 49:167, 69:140]
FROM tomo_cubed_64 AS img
// [10]: [11:129, 111:229, 23:94]
SELECT img[11:129, 111:229, 23:94]
FROM tomo_cubed_64 AS img
// [10]: [46:164, 66:184, 2:73]
SELECT img[46:164, 66:184, 2:73]
FROM tomo_cubed_64 AS img
// [10]: [88:206, 88:206, 39:110]
SELECT img[88:206, 88:206, 39:110]
FROM tomo_cubed_64 AS img
// [10]: [46:164, 22:140, 43:114]
SELECT img[46:164, 22:140, 43:114]
FROM tomo_cubed_64 AS img
// [10]: [4:122, 59:177, 47:118]
SELECT img[4:122, 59:177, 47:118]
FROM tomo_cubed_64 AS img
// [10]: [22:140, 37:155, 31:102]
SELECT img[22:140, 37:155, 31:102]
FROM tomo_cubed_64 AS img
// [10]: [109:227, 71:189, 81:152]
SELECT img[109:227, 71:189, 81:152]
FROM tomo_cubed_64 AS img
// [10]: [96:214, 76:194, 67:138]
SELECT img[96:214, 76:194, 67:138]
FROM tomo_cubed_64 AS img
// [10]: [4:122, 126:244, 10:81]
SELECT img[4:122, 126:244, 10:81]
FROM tomo_cubed_64 AS img
// [10]: [38:156, 90:208, 31:102]
SELECT img[38:156, 90:208, 31:102]
FROM tomo_cubed_64 AS img
// [10]: [12:130, 67:185, 70:141]
SELECT img[12:130, 67:185, 70:141]
FROM tomo_cubed_64 AS img
// [10]: [108:226, 59:177, 70:141]
SELECT img[108:226, 59:177, 70:141]
FROM tomo_cubed_64 AS img
// [20]: [60:209, 12:161, 15:104]
SELECT img[60:209, 12:161, 15:104]
FROM tomo_cubed_64 AS img
// [20]: [36:185, 68:217, 31:120]
SELECT img[36:185, 68:217, 31:120]
FROM tomo_cubed_64 AS img
// [20]: [19:168, 96:245, 50:139]
SELECT img[19:168, 96:245, 50:139]
FROM tomo_cubed_64 AS img
// [20]: [68:217, 96:245, 4:93]
SELECT img[68:217, 96:245, 4:93]
FROM tomo_cubed_64 AS img
// [20]: [16:165, 44:193, 49:138]
SELECT img[16:165, 44:193, 49:138]
FROM tomo_cubed_64 AS img
// [20]: [71:220, 91:240, 11:100]
SELECT img[71:220, 91:240, 11:100]
FROM tomo_cubed_64 AS img
// [20]: [96:245, 58:207, 4:93]
SELECT img[96:245, 58:207, 4:93]
FROM tomo_cubed_64 AS img
// [20]: [40:189, 6:155, 11:100]
SELECT img[40:189, 6:155, 11:100]
FROM tomo_cubed_64 AS img
// [20]: [91:240, 22:171, 7:96]
SELECT img[91:240, 22:171, 7:96]
FROM tomo_cubed_64 AS img
// [20]: [79:228, 85:234, 39:128]
SELECT img[79:228, 85:234, 39:128]
FROM tomo_cubed_64 AS img
// [20]: [97:246, 104:253, 6:95]
SELECT img[97:246, 104:253, 6:95]
FROM tomo_cubed_64 AS img
// [20]: [62:211, 26:175, 7:96]
SELECT img[62:211, 26:175, 7:96]
FROM tomo_cubed_64 AS img
// [20]: [35:184, 5:154, 29:118]
SELECT img[35:184, 5:154, 29:118]
FROM tomo_cubed_64 AS img
// [20]: [20:169, 92:241, 5:94]
SELECT img[20:169, 92:241, 5:94]
FROM tomo_cubed_64 AS img
// [20]: [77:226, 33:182, 45:134]
SELECT img[77:226, 33:182, 45:134]
FROM tomo_cubed_64 AS img
// [20]: [6:155, 40:189, 55:144]
SELECT img[6:155, 40:189, 55:144]
FROM tomo_cubed_64 AS img
// [20]: [102:251, 25:174, 20:109]
SELECT img[102:251, 25:174, 20:109]
FROM tomo_cubed_64 AS img
// [20]: [35:184, 70:219, 31:120]
SELECT img[35:184, 70:219, 31:120]
FROM tomo_cubed_64 AS img
// [20]: [99:248, 88:237, 10:99]
SELECT img[99:248, 88:237, 10:99]
FROM tomo_cubed_64 AS img
// [20]: [67:216, 63:212, 60:149]
SELECT img[67:216, 63:212, 60:149]
FROM tomo_cubed_64 AS img
// [50]: [46:248, 7:209, 31:152]
SELECT img[46:248, 7:209, 31:152]
FROM tomo_cubed_64 AS img
// [50]: [45:247, 24:226, 17:138]
SELECT img[45:247, 24:226, 17:138]
FROM tomo_cubed_64 AS img
// [50]: [22:224, 30:232, 9:130]
SELECT img[22:224, 30:232, 9:130]
FROM tomo_cubed_64 AS img
// [50]: [29:231, 39:241, 4:125]
SELECT img[29:231, 39:241, 4:125]
FROM tomo_cubed_64 AS img
// [50]: [19:221, 51:253, 6:127]
SELECT img[19:221, 51:253, 6:127]
FROM tomo_cubed_64 AS img
// [50]: [27:229, 14:216, 10:131]
SELECT img[27:229, 14:216, 10:131]
FROM tomo_cubed_64 AS img
// [50]: [33:235, 15:217, 26:147]
SELECT img[33:235, 15:217, 26:147]
FROM tomo_cubed_64 AS img
// [50]: [48:250, 1:203, 18:139]
SELECT img[48:250, 1:203, 18:139]
FROM tomo_cubed_64 AS img
// [50]: [24:226, 23:225, 23:144]
SELECT img[24:226, 23:225, 23:144]
FROM tomo_cubed_64 AS img
// [50]: [31:233, 12:214, 8:129]
SELECT img[31:233, 12:214, 8:129]
FROM tomo_cubed_64 AS img
// [50]: [34:236, 1:203, 24:145]
SELECT img[34:236, 1:203, 24:145]
FROM tomo_cubed_64 AS img
// [50]: [27:229, 17:219, 22:143]
SELECT img[27:229, 17:219, 22:143]
FROM tomo_cubed_64 AS img
// [50]: [15:217, 50:252, 16:137]
SELECT img[15:217, 50:252, 16:137]
FROM tomo_cubed_64 AS img
// [50]: [49:251, 3:205, 29:150]
SELECT img[49:251, 3:205, 29:150]
FROM tomo_cubed_64 AS img
// [50]: [11:213, 33:235, 17:138]
SELECT img[11:213, 33:235, 17:138]
FROM tomo_cubed_64 AS img
// [50]: [15:217, 23:225, 29:150]
SELECT img[15:217, 23:225, 29:150]
FROM tomo_cubed_64 AS img
// [50]: [46:248, 28:230, 30:151]
SELECT img[46:248, 28:230, 30:151]
FROM tomo_cubed_64 AS img
// [50]: [11:213, 6:208, 1:122]
SELECT img[11:213, 6:208, 1:122]
FROM tomo_cubed_64 AS img
// [50]: [13:215, 16:218, 24:145]
SELECT img[13:215, 16:218, 24:145]
FROM tomo_cubed_64 AS img
// [50]: [44:246, 46:248, 28:149]
SELECT img[44:246, 46:248, 28:149]
FROM tomo_cubed_64 AS img
