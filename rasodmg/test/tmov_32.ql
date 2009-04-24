// This query tests different selectivities with moving query boxes
// on tomo_cubed with 32kB tiles ([0:31,0:31,0:31]). Each selectivity
// is repeated 20 times with query boxes of the same shape and size
// starting at different origins.

// [0.5]: [46:90, 176:220, 60:86]
SELECT img[46:90, 176:220, 60:86]
FROM tomo_cubed AS img
// [0.5]: [143:187, 175:219, 106:132]
SELECT img[143:187, 175:219, 106:132]
FROM tomo_cubed AS img
// [0.5]: [169:213, 75:119, 66:92]
SELECT img[169:213, 75:119, 66:92]
FROM tomo_cubed AS img
// [0.5]: [90:134, 178:222, 48:74]
SELECT img[90:134, 178:222, 48:74]
FROM tomo_cubed AS img
// [0.5]: [76:120, 136:180, 31:57]
SELECT img[76:120, 136:180, 31:57]
FROM tomo_cubed AS img
// [0.5]: [7:51, 150:194, 89:115]
SELECT img[7:51, 150:194, 89:115]
FROM tomo_cubed AS img
// [0.5]: [133:177, 113:157, 0:26]
SELECT img[133:177, 113:157, 0:26]
FROM tomo_cubed AS img
// [0.5]: [114:158, 125:169, 108:134]
SELECT img[114:158, 125:169, 108:134]
FROM tomo_cubed AS img
// [0.5]: [210:254, 12:56, 56:82]
SELECT img[210:254, 12:56, 56:82]
FROM tomo_cubed AS img
// [0.5]: [176:220, 171:215, 83:109]
SELECT img[176:220, 171:215, 83:109]
FROM tomo_cubed AS img
// [0.5]: [102:146, 50:94, 34:60]
SELECT img[102:146, 50:94, 34:60]
FROM tomo_cubed AS img
// [0.5]: [134:178, 96:140, 97:123]
SELECT img[134:178, 96:140, 97:123]
FROM tomo_cubed AS img
// [0.5]: [200:244, 43:87, 123:149]
SELECT img[200:244, 43:87, 123:149]
FROM tomo_cubed AS img
// [0.5]: [155:199, 127:171, 48:74]
SELECT img[155:199, 127:171, 48:74]
FROM tomo_cubed AS img
// [0.5]: [111:155, 80:124, 11:37]
SELECT img[111:155, 80:124, 11:37]
FROM tomo_cubed AS img
// [0.5]: [164:208, 2:46, 64:90]
SELECT img[164:208, 2:46, 64:90]
FROM tomo_cubed AS img
// [0.5]: [137:181, 102:146, 17:43]
SELECT img[137:181, 102:146, 17:43]
FROM tomo_cubed AS img
// [0.5]: [100:144, 149:193, 121:147]
SELECT img[100:144, 149:193, 121:147]
FROM tomo_cubed AS img
// [0.5]: [186:230, 38:82, 108:134]
SELECT img[186:230, 38:82, 108:134]
FROM tomo_cubed AS img
// [0.5]: [48:92, 53:97, 14:40]
SELECT img[48:92, 53:97, 14:40]
FROM tomo_cubed AS img
// [1]: [64:119, 65:120, 57:90]
SELECT img[64:119, 65:120, 57:90]
FROM tomo_cubed AS img
// [1]: [13:68, 121:176, 20:53]
SELECT img[13:68, 121:176, 20:53]
FROM tomo_cubed AS img
// [1]: [3:58, 115:170, 59:92]
SELECT img[3:58, 115:170, 59:92]
FROM tomo_cubed AS img
// [1]: [174:229, 82:137, 93:126]
SELECT img[174:229, 82:137, 93:126]
FROM tomo_cubed AS img
// [1]: [40:95, 143:198, 43:76]
SELECT img[40:95, 143:198, 43:76]
FROM tomo_cubed AS img
// [1]: [126:181, 160:215, 86:119]
SELECT img[126:181, 160:215, 86:119]
FROM tomo_cubed AS img
// [1]: [20:75, 44:99, 89:122]
SELECT img[20:75, 44:99, 89:122]
FROM tomo_cubed AS img
// [1]: [15:70, 92:147, 23:56]
SELECT img[15:70, 92:147, 23:56]
FROM tomo_cubed AS img
// [1]: [122:177, 62:117, 75:108]
SELECT img[122:177, 62:117, 75:108]
FROM tomo_cubed AS img
// [1]: [183:238, 174:229, 70:103]
SELECT img[183:238, 174:229, 70:103]
FROM tomo_cubed AS img
// [1]: [13:68, 82:137, 97:130]
SELECT img[13:68, 82:137, 97:130]
FROM tomo_cubed AS img
// [1]: [174:229, 107:162, 91:124]
SELECT img[174:229, 107:162, 91:124]
FROM tomo_cubed AS img
// [1]: [79:134, 83:138, 88:121]
SELECT img[79:134, 83:138, 88:121]
FROM tomo_cubed AS img
// [1]: [3:58, 62:117, 39:72]
SELECT img[3:58, 62:117, 39:72]
FROM tomo_cubed AS img
// [1]: [96:151, 25:80, 93:126]
SELECT img[96:151, 25:80, 93:126]
FROM tomo_cubed AS img
// [1]: [124:179, 183:238, 21:54]
SELECT img[124:179, 183:238, 21:54]
FROM tomo_cubed AS img
// [1]: [106:161, 111:166, 89:122]
SELECT img[106:161, 111:166, 89:122]
FROM tomo_cubed AS img
// [1]: [66:121, 103:158, 74:107]
SELECT img[66:121, 103:158, 74:107]
FROM tomo_cubed AS img
// [1]: [180:235, 131:186, 80:113]
SELECT img[180:235, 131:186, 80:113]
FROM tomo_cubed AS img
// [1]: [4:59, 132:187, 59:92]
SELECT img[4:59, 132:187, 59:92]
FROM tomo_cubed AS img
// [2]: [69:138, 184:253, 75:117]
SELECT img[69:138, 184:253, 75:117]
FROM tomo_cubed AS img
// [2]: [58:127, 168:237, 87:129]
SELECT img[58:127, 168:237, 87:129]
FROM tomo_cubed AS img
// [2]: [162:231, 145:214, 58:100]
SELECT img[162:231, 145:214, 58:100]
FROM tomo_cubed AS img
// [2]: [129:198, 122:191, 7:49]
SELECT img[129:198, 122:191, 7:49]
FROM tomo_cubed AS img
// [2]: [86:155, 170:239, 54:96]
SELECT img[86:155, 170:239, 54:96]
FROM tomo_cubed AS img
// [2]: [0:69, 98:167, 70:112]
SELECT img[0:69, 98:167, 70:112]
FROM tomo_cubed AS img
// [2]: [41:110, 33:102, 58:100]
SELECT img[41:110, 33:102, 58:100]
FROM tomo_cubed AS img
// [2]: [37:106, 24:93, 65:107]
SELECT img[37:106, 24:93, 65:107]
FROM tomo_cubed AS img
// [2]: [39:108, 114:183, 26:68]
SELECT img[39:108, 114:183, 26:68]
FROM tomo_cubed AS img
// [2]: [166:235, 9:78, 38:80]
SELECT img[166:235, 9:78, 38:80]
FROM tomo_cubed AS img
// [2]: [147:216, 174:243, 45:87]
SELECT img[147:216, 174:243, 45:87]
FROM tomo_cubed AS img
// [2]: [121:190, 69:138, 58:100]
SELECT img[121:190, 69:138, 58:100]
FROM tomo_cubed AS img
// [2]: [87:156, 78:147, 73:115]
SELECT img[87:156, 78:147, 73:115]
FROM tomo_cubed AS img
// [2]: [31:100, 109:178, 75:117]
SELECT img[31:100, 109:178, 75:117]
FROM tomo_cubed AS img
// [2]: [54:123, 155:224, 26:68]
SELECT img[54:123, 155:224, 26:68]
FROM tomo_cubed AS img
// [2]: [150:219, 139:208, 25:67]
SELECT img[150:219, 139:208, 25:67]
FROM tomo_cubed AS img
// [2]: [65:134, 119:188, 87:129]
SELECT img[65:134, 119:188, 87:129]
FROM tomo_cubed AS img
// [2]: [159:228, 45:114, 47:89]
SELECT img[159:228, 45:114, 47:89]
FROM tomo_cubed AS img
// [2]: [77:146, 60:129, 87:129]
SELECT img[77:146, 60:129, 87:129]
FROM tomo_cubed AS img
// [2]: [169:238, 135:204, 4:46]
SELECT img[169:238, 135:204, 4:46]
FROM tomo_cubed AS img
// [5]: [24:118, 61:155, 77:133]
SELECT img[24:118, 61:155, 77:133]
FROM tomo_cubed AS img
// [5]: [137:231, 119:213, 11:67]
SELECT img[137:231, 119:213, 11:67]
FROM tomo_cubed AS img
// [5]: [140:234, 131:225, 56:112]
SELECT img[140:234, 131:225, 56:112]
FROM tomo_cubed AS img
// [5]: [154:248, 6:100, 54:110]
SELECT img[154:248, 6:100, 54:110]
FROM tomo_cubed AS img
// [5]: [146:240, 62:156, 60:116]
SELECT img[146:240, 62:156, 60:116]
FROM tomo_cubed AS img
// [5]: [78:172, 21:115, 3:59]
SELECT img[78:172, 21:115, 3:59]
FROM tomo_cubed AS img
// [5]: [80:174, 33:127, 84:140]
SELECT img[80:174, 33:127, 84:140]
FROM tomo_cubed AS img
// [5]: [138:232, 95:189, 52:108]
SELECT img[138:232, 95:189, 52:108]
FROM tomo_cubed AS img
// [5]: [60:154, 136:230, 70:126]
SELECT img[60:154, 136:230, 70:126]
FROM tomo_cubed AS img
// [5]: [79:173, 149:243, 65:121]
SELECT img[79:173, 149:243, 65:121]
FROM tomo_cubed AS img
// [5]: [13:107, 53:147, 57:113]
SELECT img[13:107, 53:147, 57:113]
FROM tomo_cubed AS img
// [5]: [13:107, 66:160, 44:100]
SELECT img[13:107, 66:160, 44:100]
FROM tomo_cubed AS img
// [5]: [137:231, 47:141, 38:94]
SELECT img[137:231, 47:141, 38:94]
FROM tomo_cubed AS img
// [5]: [11:105, 84:178, 42:98]
SELECT img[11:105, 84:178, 42:98]
FROM tomo_cubed AS img
// [5]: [19:113, 31:125, 51:107]
SELECT img[19:113, 31:125, 51:107]
FROM tomo_cubed AS img
// [5]: [27:121, 17:111, 29:85]
SELECT img[27:121, 17:111, 29:85]
FROM tomo_cubed AS img
// [5]: [142:236, 76:170, 94:150]
SELECT img[142:236, 76:170, 94:150]
FROM tomo_cubed AS img
// [5]: [153:247, 71:165, 83:139]
SELECT img[153:247, 71:165, 83:139]
FROM tomo_cubed AS img
// [5]: [12:106, 108:202, 79:135]
SELECT img[12:106, 108:202, 79:135]
FROM tomo_cubed AS img
// [5]: [146:240, 26:120, 59:115]
SELECT img[146:240, 26:120, 59:115]
FROM tomo_cubed AS img
// 1[0]: [3:121, 62:180, 39:110]
SELECT img[3:121, 62:180, 39:110]
FROM tomo_cubed AS img
// 1[0]: [11:129, 26:144, 19:90]
SELECT img[11:129, 26:144, 19:90]
FROM tomo_cubed AS img
// 1[0]: [49:167, 76:194, 77:148]
SELECT img[49:167, 76:194, 77:148]
FROM tomo_cubed AS img
// [10]: [55:173, 86:204, 21:92]
SELECT img[55:173, 86:204, 21:92]
FROM tomo_cubed AS img
// [10]: [110:228, 98:216, 80:151]
SELECT img[110:228, 98:216, 80:151]
FROM tomo_cubed AS img
// [10]: [123:241, 28:146, 59:130]
SELECT img[123:241, 28:146, 59:130]
FROM tomo_cubed AS img
// [10]: [89:207, 90:208, 34:105]
SELECT img[89:207, 90:208, 34:105]
FROM tomo_cubed AS img
// [10]: [111:229, 51:169, 5:76]
SELECT img[111:229, 51:169, 5:76]
FROM tomo_cubed AS img
// [10]: [15:133, 74:192, 12:83]
SELECT img[15:133, 74:192, 12:83]
FROM tomo_cubed AS img
// [10]: [103:221, 94:212, 74:145]
SELECT img[103:221, 94:212, 74:145]
FROM tomo_cubed AS img
// [10]: [107:225, 111:229, 16:87]
SELECT img[107:225, 111:229, 16:87]
FROM tomo_cubed AS img
// [10]: [14:132, 80:198, 58:129]
SELECT img[14:132, 80:198, 58:129]
FROM tomo_cubed AS img
// [10]: [2:120, 64:182, 25:96]
SELECT img[2:120, 64:182, 25:96]
FROM tomo_cubed AS img
// [10]: [74:192, 130:248, 3:74]
SELECT img[74:192, 130:248, 3:74]
FROM tomo_cubed AS img
// [10]: [96:214, 61:179, 14:85]
SELECT img[96:214, 61:179, 14:85]
FROM tomo_cubed AS img
// [10]: [73:191, 93:211, 6:77]
SELECT img[73:191, 93:211, 6:77]
FROM tomo_cubed AS img
// [10]: [55:173, 12:130, 54:125]
SELECT img[55:173, 12:130, 54:125]
FROM tomo_cubed AS img
// [10]: [1:119, 123:241, 51:122]
SELECT img[1:119, 123:241, 51:122]
FROM tomo_cubed AS img
// [10]: [117:235, 101:219, 55:126]
SELECT img[117:235, 101:219, 55:126]
FROM tomo_cubed AS img
// [10]: [24:142, 26:144, 33:104]
SELECT img[24:142, 26:144, 33:104]
FROM tomo_cubed AS img
// [20]: [36:185, 0:149, 39:128]
SELECT img[36:185, 0:149, 39:128]
FROM tomo_cubed AS img
// [20]: [85:234, 31:180, 54:143]
SELECT img[85:234, 31:180, 54:143]
FROM tomo_cubed AS img
// [20]: [55:204, 78:227, 20:109]
SELECT img[55:204, 78:227, 20:109]
FROM tomo_cubed AS img
// [20]: [28:177, 98:247, 26:115]
SELECT img[28:177, 98:247, 26:115]
FROM tomo_cubed AS img
// [20]: [53:202, 14:163, 15:104]
SELECT img[53:202, 14:163, 15:104]
FROM tomo_cubed AS img
// [20]: [84:233, 59:208, 28:117]
SELECT img[84:233, 59:208, 28:117]
FROM tomo_cubed AS img
// [20]: [24:173, 81:230, 21:110]
SELECT img[24:173, 81:230, 21:110]
FROM tomo_cubed AS img
// [20]: [70:219, 103:252, 59:148]
SELECT img[70:219, 103:252, 59:148]
FROM tomo_cubed AS img
// [20]: [75:224, 32:181, 36:125]
SELECT img[75:224, 32:181, 36:125]
FROM tomo_cubed AS img
// [20]: [80:229, 65:214, 22:111]
SELECT img[80:229, 65:214, 22:111]
FROM tomo_cubed AS img
// [20]: [5:154, 34:183, 20:109]
SELECT img[5:154, 34:183, 20:109]
FROM tomo_cubed AS img
// [20]: [16:165, 72:221, 57:146]
SELECT img[16:165, 72:221, 57:146]
FROM tomo_cubed AS img
// [20]: [90:239, 27:176, 55:144]
SELECT img[90:239, 27:176, 55:144]
FROM tomo_cubed AS img
// [20]: [81:230, 63:212, 61:150]
SELECT img[81:230, 63:212, 61:150]
FROM tomo_cubed AS img
// [20]: [87:236, 79:228, 61:150]
SELECT img[87:236, 79:228, 61:150]
FROM tomo_cubed AS img
// [20]: [100:249, 33:182, 35:124]
SELECT img[100:249, 33:182, 35:124]
FROM tomo_cubed AS img
// [20]: [14:163, 91:240, 38:127]
SELECT img[14:163, 91:240, 38:127]
FROM tomo_cubed AS img
// [20]: [60:209, 95:244, 11:100]
SELECT img[60:209, 95:244, 11:100]
FROM tomo_cubed AS img
// [20]: [99:248, 4:153, 29:118]
SELECT img[99:248, 4:153, 29:118]
FROM tomo_cubed AS img
// [20]: [10:159, 64:213, 32:121]
SELECT img[10:159, 64:213, 32:121]
FROM tomo_cubed AS img
// [50]: [8:210, 0:202, 1:122]
SELECT img[8:210, 0:202, 1:122]
FROM tomo_cubed AS img
// [50]: [17:219, 21:223, 6:127]
SELECT img[17:219, 21:223, 6:127]
FROM tomo_cubed AS img
// [50]: [49:251, 0:202, 4:125]
SELECT img[49:251, 0:202, 4:125]
FROM tomo_cubed AS img
// [50]: [11:213, 37:239, 15:136]
SELECT img[11:213, 37:239, 15:136]
FROM tomo_cubed AS img
// [50]: [29:231, 36:238, 18:139]
SELECT img[29:231, 36:238, 18:139]
FROM tomo_cubed AS img
// [50]: [38:240, 21:223, 2:123]
SELECT img[38:240, 21:223, 2:123]
FROM tomo_cubed AS img
// [50]: [26:228, 4:206, 9:130]
SELECT img[26:228, 4:206, 9:130]
FROM tomo_cubed AS img
// [50]: [1:203, 41:243, 21:142]
SELECT img[1:203, 41:243, 21:142]
FROM tomo_cubed AS img
// [50]: [39:241, 37:239, 10:131]
SELECT img[39:241, 37:239, 10:131]
FROM tomo_cubed AS img
// [50]: [6:208, 49:251, 2:123]
SELECT img[6:208, 49:251, 2:123]
FROM tomo_cubed AS img
// [50]: [2:204, 51:253, 8:129]
SELECT img[2:204, 51:253, 8:129]
FROM tomo_cubed AS img
// [50]: [23:225, 51:253, 9:130]
SELECT img[23:225, 51:253, 9:130]
FROM tomo_cubed AS img
// [50]: [32:234, 43:245, 10:131]
SELECT img[32:234, 43:245, 10:131]
FROM tomo_cubed AS img
// [50]: [33:235, 9:211, 31:152]
SELECT img[33:235, 9:211, 31:152]
FROM tomo_cubed AS img
// [50]: [28:230, 36:238, 4:125]
SELECT img[28:230, 36:238, 4:125]
FROM tomo_cubed AS img
// [50]: [25:227, 40:242, 3:124]
SELECT img[25:227, 40:242, 3:124]
FROM tomo_cubed AS img
// [50]: [33:235, 28:230, 25:146]
SELECT img[33:235, 28:230, 25:146]
FROM tomo_cubed AS img
// [50]: [42:244, 34:236, 1:122]
SELECT img[42:244, 34:236, 1:122]
FROM tomo_cubed AS img
// [50]: [34:236, 47:249, 6:127]
SELECT img[34:236, 47:249, 6:127]
FROM tomo_cubed AS img
// [50]: [35:237, 22:224, 31:152]
SELECT img[35:237, 22:224, 31:152]
FROM tomo_cubed AS img
