// This query tests different selectivities with moving query boxes
// on comp_cubed with 16kB tiles ([0:24,0:24,0:24]). Each selectivity
// is repeated 20 times with query boxes of the same shape and size
// starting at different origins.

// [0.5]: [39:83, 43:87, 117:143]
SELECT img[39:83, 43:87, 117:143]
FROM comp_cubed_16 AS img
// [0.5]: [146:190, 116:160, 35:61]
SELECT img[146:190, 116:160, 35:61]
FROM comp_cubed_16 AS img
// [0.5]: [17:61, 42:86, 15:41]
SELECT img[17:61, 42:86, 15:41]
FROM comp_cubed_16 AS img
// [0.5]: [112:156, 154:198, 108:134]
SELECT img[112:156, 154:198, 108:134]
FROM comp_cubed_16 AS img
// [0.5]: [138:182, 26:70, 48:74]
SELECT img[138:182, 26:70, 48:74]
FROM comp_cubed_16 AS img
// [0.5]: [53:97, 94:138, 22:48]
SELECT img[53:97, 94:138, 22:48]
FROM comp_cubed_16 AS img
// [0.5]: [190:234, 59:103, 17:43]
SELECT img[190:234, 59:103, 17:43]
FROM comp_cubed_16 AS img
// [0.5]: [118:162, 147:191, 30:56]
SELECT img[118:162, 147:191, 30:56]
FROM comp_cubed_16 AS img
// [0.5]: [210:254, 179:223, 18:44]
SELECT img[210:254, 179:223, 18:44]
FROM comp_cubed_16 AS img
// [0.5]: [129:173, 76:120, 102:128]
SELECT img[129:173, 76:120, 102:128]
FROM comp_cubed_16 AS img
// [0.5]: [199:243, 119:163, 59:85]
SELECT img[199:243, 119:163, 59:85]
FROM comp_cubed_16 AS img
// [0.5]: [47:91, 52:96, 108:134]
SELECT img[47:91, 52:96, 108:134]
FROM comp_cubed_16 AS img
// [0.5]: [87:131, 103:147, 11:37]
SELECT img[87:131, 103:147, 11:37]
FROM comp_cubed_16 AS img
// [0.5]: [32:76, 81:125, 87:113]
SELECT img[32:76, 81:125, 87:113]
FROM comp_cubed_16 AS img
// [0.5]: [14:58, 77:121, 0:26]
SELECT img[14:58, 77:121, 0:26]
FROM comp_cubed_16 AS img
// [0.5]: [128:172, 145:189, 21:47]
SELECT img[128:172, 145:189, 21:47]
FROM comp_cubed_16 AS img
// [0.5]: [185:229, 102:146, 60:86]
SELECT img[185:229, 102:146, 60:86]
FROM comp_cubed_16 AS img
// [0.5]: [40:84, 39:83, 106:132]
SELECT img[40:84, 39:83, 106:132]
FROM comp_cubed_16 AS img
// [0.5]: [37:81, 10:54, 51:77]
SELECT img[37:81, 10:54, 51:77]
FROM comp_cubed_16 AS img
// [0.5]: [46:90, 21:65, 102:128]
SELECT img[46:90, 21:65, 102:128]
FROM comp_cubed_16 AS img
// [1]: [199:254, 50:105, 11:44]
SELECT img[199:254, 50:105, 11:44]
FROM comp_cubed_16 AS img
// [1]: [180:235, 51:106, 91:124]
SELECT img[180:235, 51:106, 91:124]
FROM comp_cubed_16 AS img
// [1]: [199:254, 43:98, 113:146]
SELECT img[199:254, 43:98, 113:146]
FROM comp_cubed_16 AS img
// [1]: [78:133, 40:95, 37:70]
SELECT img[78:133, 40:95, 37:70]
FROM comp_cubed_16 AS img
// [1]: [11:66, 98:153, 19:52]
SELECT img[11:66, 98:153, 19:52]
FROM comp_cubed_16 AS img
// [1]: [90:145, 74:129, 83:116]
SELECT img[90:145, 74:129, 83:116]
FROM comp_cubed_16 AS img
// [1]: [82:137, 103:158, 1:34]
SELECT img[82:137, 103:158, 1:34]
FROM comp_cubed_16 AS img
// [1]: [186:241, 191:246, 47:80]
SELECT img[186:241, 191:246, 47:80]
FROM comp_cubed_16 AS img
// [1]: [143:198, 68:123, 77:110]
SELECT img[143:198, 68:123, 77:110]
FROM comp_cubed_16 AS img
// [1]: [188:243, 23:78, 94:127]
SELECT img[188:243, 23:78, 94:127]
FROM comp_cubed_16 AS img
// [1]: [71:126, 11:66, 75:108]
SELECT img[71:126, 11:66, 75:108]
FROM comp_cubed_16 AS img
// [1]: [89:144, 109:164, 12:45]
SELECT img[89:144, 109:164, 12:45]
FROM comp_cubed_16 AS img
// [1]: [47:102, 56:111, 14:47]
SELECT img[47:102, 56:111, 14:47]
FROM comp_cubed_16 AS img
// [1]: [24:79, 190:245, 77:110]
SELECT img[24:79, 190:245, 77:110]
FROM comp_cubed_16 AS img
// [1]: [14:69, 189:244, 0:33]
SELECT img[14:69, 189:244, 0:33]
FROM comp_cubed_16 AS img
// [1]: [198:253, 167:222, 117:150]
SELECT img[198:253, 167:222, 117:150]
FROM comp_cubed_16 AS img
// [1]: [125:180, 1:56, 113:146]
SELECT img[125:180, 1:56, 113:146]
FROM comp_cubed_16 AS img
// [1]: [90:145, 126:181, 109:142]
SELECT img[90:145, 126:181, 109:142]
FROM comp_cubed_16 AS img
// [1]: [154:209, 9:64, 47:80]
SELECT img[154:209, 9:64, 47:80]
FROM comp_cubed_16 AS img
// [1]: [81:136, 1:56, 87:120]
SELECT img[81:136, 1:56, 87:120]
FROM comp_cubed_16 AS img
// [2]: [116:185, 1:70, 77:119]
SELECT img[116:185, 1:70, 77:119]
FROM comp_cubed_16 AS img
// [2]: [99:168, 87:156, 20:62]
SELECT img[99:168, 87:156, 20:62]
FROM comp_cubed_16 AS img
// [2]: [81:150, 74:143, 35:77]
SELECT img[81:150, 74:143, 35:77]
FROM comp_cubed_16 AS img
// [2]: [16:85, 100:169, 56:98]
SELECT img[16:85, 100:169, 56:98]
FROM comp_cubed_16 AS img
// [2]: [180:249, 47:116, 92:134]
SELECT img[180:249, 47:116, 92:134]
FROM comp_cubed_16 AS img
// [2]: [78:147, 57:126, 9:51]
SELECT img[78:147, 57:126, 9:51]
FROM comp_cubed_16 AS img
// [2]: [160:229, 76:145, 39:81]
SELECT img[160:229, 76:145, 39:81]
FROM comp_cubed_16 AS img
// [2]: [95:164, 29:98, 26:68]
SELECT img[95:164, 29:98, 26:68]
FROM comp_cubed_16 AS img
// [2]: [143:212, 120:189, 35:77]
SELECT img[143:212, 120:189, 35:77]
FROM comp_cubed_16 AS img
// [2]: [163:232, 116:185, 78:120]
SELECT img[163:232, 116:185, 78:120]
FROM comp_cubed_16 AS img
// [2]: [116:185, 58:127, 69:111]
SELECT img[116:185, 58:127, 69:111]
FROM comp_cubed_16 AS img
// [2]: [38:107, 169:238, 0:42]
SELECT img[38:107, 169:238, 0:42]
FROM comp_cubed_16 AS img
// [2]: [87:156, 112:181, 41:83]
SELECT img[87:156, 112:181, 41:83]
FROM comp_cubed_16 AS img
// [2]: [14:83, 118:187, 37:79]
SELECT img[14:83, 118:187, 37:79]
FROM comp_cubed_16 AS img
// [2]: [24:93, 172:241, 27:69]
SELECT img[24:93, 172:241, 27:69]
FROM comp_cubed_16 AS img
// [2]: [105:174, 14:83, 21:63]
SELECT img[105:174, 14:83, 21:63]
FROM comp_cubed_16 AS img
// [2]: [54:123, 92:161, 0:42]
SELECT img[54:123, 92:161, 0:42]
FROM comp_cubed_16 AS img
// [2]: [151:220, 160:229, 58:100]
SELECT img[151:220, 160:229, 58:100]
FROM comp_cubed_16 AS img
// [2]: [73:142, 42:111, 73:115]
SELECT img[73:142, 42:111, 73:115]
FROM comp_cubed_16 AS img
// [2]: [38:107, 181:250, 25:67]
SELECT img[38:107, 181:250, 25:67]
FROM comp_cubed_16 AS img
// [5]: [82:176, 43:137, 32:88]
SELECT img[82:176, 43:137, 32:88]
FROM comp_cubed_16 AS img
// [5]: [114:208, 6:100, 52:108]
SELECT img[114:208, 6:100, 52:108]
FROM comp_cubed_16 AS img
// [5]: [116:210, 132:226, 29:85]
SELECT img[116:210, 132:226, 29:85]
FROM comp_cubed_16 AS img
// [5]: [29:123, 69:163, 53:109]
SELECT img[29:123, 69:163, 53:109]
FROM comp_cubed_16 AS img
// [5]: [85:179, 1:95, 72:128]
SELECT img[85:179, 1:95, 72:128]
FROM comp_cubed_16 AS img
// [5]: [80:174, 121:215, 27:83]
SELECT img[80:174, 121:215, 27:83]
FROM comp_cubed_16 AS img
// [5]: [142:236, 129:223, 46:102]
SELECT img[142:236, 129:223, 46:102]
FROM comp_cubed_16 AS img
// [5]: [130:224, 156:250, 82:138]
SELECT img[130:224, 156:250, 82:138]
FROM comp_cubed_16 AS img
// [5]: [27:121, 111:205, 45:101]
SELECT img[27:121, 111:205, 45:101]
FROM comp_cubed_16 AS img
// [5]: [139:233, 122:216, 49:105]
SELECT img[139:233, 122:216, 49:105]
FROM comp_cubed_16 AS img
// [5]: [12:106, 3:97, 84:140]
SELECT img[12:106, 3:97, 84:140]
FROM comp_cubed_16 AS img
// [5]: [73:167, 70:164, 1:57]
SELECT img[73:167, 70:164, 1:57]
FROM comp_cubed_16 AS img
// [5]: [160:254, 1:95, 80:136]
SELECT img[160:254, 1:95, 80:136]
FROM comp_cubed_16 AS img
// [5]: [152:246, 54:148, 72:128]
SELECT img[152:246, 54:148, 72:128]
FROM comp_cubed_16 AS img
// [5]: [25:119, 50:144, 55:111]
SELECT img[25:119, 50:144, 55:111]
FROM comp_cubed_16 AS img
// [5]: [151:245, 34:128, 92:148]
SELECT img[151:245, 34:128, 92:148]
FROM comp_cubed_16 AS img
// [5]: [28:122, 68:162, 76:132]
SELECT img[28:122, 68:162, 76:132]
FROM comp_cubed_16 AS img
// [5]: [145:239, 98:192, 44:100]
SELECT img[145:239, 98:192, 44:100]
FROM comp_cubed_16 AS img
// [5]: [73:167, 122:216, 2:58]
SELECT img[73:167, 122:216, 2:58]
FROM comp_cubed_16 AS img
// [5]: [1:95, 107:201, 68:124]
SELECT img[1:95, 107:201, 68:124]
FROM comp_cubed_16 AS img
// [10]: [102:220, 43:161, 75:146]
SELECT img[102:220, 43:161, 75:146]
FROM comp_cubed_16 AS img
// [10]: [106:224, 36:154, 79:150]
SELECT img[106:224, 36:154, 79:150]
FROM comp_cubed_16 AS img
// [10]: [95:213, 56:174, 69:140]
SELECT img[95:213, 56:174, 69:140]
FROM comp_cubed_16 AS img
// [10]: [62:180, 126:244, 74:145]
SELECT img[62:180, 126:244, 74:145]
FROM comp_cubed_16 AS img
// [10]: [93:211, 63:181, 46:117]
SELECT img[93:211, 63:181, 46:117]
FROM comp_cubed_16 AS img
// [10]: [129:247, 74:192, 67:138]
SELECT img[129:247, 74:192, 67:138]
FROM comp_cubed_16 AS img
// [10]: [44:162, 84:202, 75:146]
SELECT img[44:162, 84:202, 75:146]
FROM comp_cubed_16 AS img
// [10]: [52:170, 107:225, 19:90]
SELECT img[52:170, 107:225, 19:90]
FROM comp_cubed_16 AS img
// [10]: [89:207, 94:212, 9:80]
SELECT img[89:207, 94:212, 9:80]
FROM comp_cubed_16 AS img
// [10]: [52:170, 120:238, 67:138]
SELECT img[52:170, 120:238, 67:138]
FROM comp_cubed_16 AS img
// [10]: [98:216, 51:169, 42:113]
SELECT img[98:216, 51:169, 42:113]
FROM comp_cubed_16 AS img
// [10]: [30:148, 109:227, 51:122]
SELECT img[30:148, 109:227, 51:122]
FROM comp_cubed_16 AS img
// [10]: [130:248, 118:236, 30:101]
SELECT img[130:248, 118:236, 30:101]
FROM comp_cubed_16 AS img
// [10]: [25:143, 25:143, 46:117]
SELECT img[25:143, 25:143, 46:117]
FROM comp_cubed_16 AS img
// [10]: [86:204, 119:237, 12:83]
SELECT img[86:204, 119:237, 12:83]
FROM comp_cubed_16 AS img
// [10]: [116:234, 18:136, 11:82]
SELECT img[116:234, 18:136, 11:82]
FROM comp_cubed_16 AS img
// [10]: [61:179, 24:142, 68:139]
SELECT img[61:179, 24:142, 68:139]
FROM comp_cubed_16 AS img
// [10]: [53:171, 14:132, 72:143]
SELECT img[53:171, 14:132, 72:143]
FROM comp_cubed_16 AS img
// [10]: [49:167, 84:202, 64:135]
SELECT img[49:167, 84:202, 64:135]
FROM comp_cubed_16 AS img
// [10]: [85:203, 80:198, 13:84]
SELECT img[85:203, 80:198, 13:84]
FROM comp_cubed_16 AS img
// [20]: [97:246, 85:234, 54:143]
SELECT img[97:246, 85:234, 54:143]
FROM comp_cubed_16 AS img
// [20]: [63:212, 18:167, 43:132]
SELECT img[63:212, 18:167, 43:132]
FROM comp_cubed_16 AS img
// [20]: [99:248, 28:177, 58:147]
SELECT img[99:248, 28:177, 58:147]
FROM comp_cubed_16 AS img
// [20]: [57:206, 16:165, 46:135]
SELECT img[57:206, 16:165, 46:135]
FROM comp_cubed_16 AS img
// [20]: [83:232, 93:242, 36:125]
SELECT img[83:232, 93:242, 36:125]
FROM comp_cubed_16 AS img
// [20]: [57:206, 31:180, 48:137]
SELECT img[57:206, 31:180, 48:137]
FROM comp_cubed_16 AS img
// [20]: [86:235, 70:219, 32:121]
SELECT img[86:235, 70:219, 32:121]
FROM comp_cubed_16 AS img
// [20]: [39:188, 49:198, 50:139]
SELECT img[39:188, 49:198, 50:139]
FROM comp_cubed_16 AS img
// [20]: [62:211, 104:253, 29:118]
SELECT img[62:211, 104:253, 29:118]
FROM comp_cubed_16 AS img
// [20]: [103:252, 44:193, 13:102]
SELECT img[103:252, 44:193, 13:102]
FROM comp_cubed_16 AS img
// [20]: [48:197, 14:163, 2:91]
SELECT img[48:197, 14:163, 2:91]
FROM comp_cubed_16 AS img
// [20]: [0:149, 79:228, 13:102]
SELECT img[0:149, 79:228, 13:102]
FROM comp_cubed_16 AS img
// [20]: [6:155, 103:252, 35:124]
SELECT img[6:155, 103:252, 35:124]
FROM comp_cubed_16 AS img
// [20]: [0:149, 91:240, 46:135]
SELECT img[0:149, 91:240, 46:135]
FROM comp_cubed_16 AS img
// [20]: [23:172, 50:199, 42:131]
SELECT img[23:172, 50:199, 42:131]
FROM comp_cubed_16 AS img
// [20]: [80:229, 33:182, 17:106]
SELECT img[80:229, 33:182, 17:106]
FROM comp_cubed_16 AS img
// [20]: [38:187, 41:190, 1:90]
SELECT img[38:187, 41:190, 1:90]
FROM comp_cubed_16 AS img
// [20]: [42:191, 17:166, 39:128]
SELECT img[42:191, 17:166, 39:128]
FROM comp_cubed_16 AS img
// [20]: [98:247, 95:244, 58:147]
SELECT img[98:247, 95:244, 58:147]
FROM comp_cubed_16 AS img
// [20]: [10:159, 51:200, 21:110]
SELECT img[10:159, 51:200, 21:110]
FROM comp_cubed_16 AS img
// [50]: [8:210, 13:215, 9:130]
SELECT img[8:210, 13:215, 9:130]
FROM comp_cubed_16 AS img
// [50]: [48:250, 45:247, 24:145]
SELECT img[48:250, 45:247, 24:145]
FROM comp_cubed_16 AS img
// [50]: [29:231, 6:208, 23:144]
SELECT img[29:231, 6:208, 23:144]
FROM comp_cubed_16 AS img
// [50]: [12:214, 44:246, 23:144]
SELECT img[12:214, 44:246, 23:144]
FROM comp_cubed_16 AS img
// [50]: [11:213, 27:229, 7:128]
SELECT img[11:213, 27:229, 7:128]
FROM comp_cubed_16 AS img
// [50]: [1:203, 2:204, 14:135]
SELECT img[1:203, 2:204, 14:135]
FROM comp_cubed_16 AS img
// [50]: [22:224, 50:252, 10:131]
SELECT img[22:224, 50:252, 10:131]
FROM comp_cubed_16 AS img
// [50]: [15:217, 35:237, 9:130]
SELECT img[15:217, 35:237, 9:130]
FROM comp_cubed_16 AS img
// [50]: [4:206, 51:253, 28:149]
SELECT img[4:206, 51:253, 28:149]
FROM comp_cubed_16 AS img
// [50]: [22:224, 20:222, 31:152]
SELECT img[22:224, 20:222, 31:152]
FROM comp_cubed_16 AS img
// [50]: [16:218, 42:244, 27:148]
SELECT img[16:218, 42:244, 27:148]
FROM comp_cubed_16 AS img
// [50]: [21:223, 32:234, 22:143]
SELECT img[21:223, 32:234, 22:143]
FROM comp_cubed_16 AS img
// [50]: [37:239, 44:246, 29:150]
SELECT img[37:239, 44:246, 29:150]
FROM comp_cubed_16 AS img
// [50]: [17:219, 49:251, 14:135]
SELECT img[17:219, 49:251, 14:135]
FROM comp_cubed_16 AS img
// [50]: [49:251, 50:252, 3:124]
SELECT img[49:251, 50:252, 3:124]
FROM comp_cubed_16 AS img
// [50]: [17:219, 15:217, 1:122]
SELECT img[17:219, 15:217, 1:122]
FROM comp_cubed_16 AS img
// [50]: [21:223, 51:253, 13:134]
SELECT img[21:223, 51:253, 13:134]
FROM comp_cubed_16 AS img
// [50]: [7:209, 47:249, 11:132]
SELECT img[7:209, 47:249, 11:132]
FROM comp_cubed_16 AS img
// [50]: [4:206, 22:224, 4:125]
SELECT img[4:206, 22:224, 4:125]
FROM comp_cubed_16 AS img
// [50]: [16:218, 51:253, 21:142]
SELECT img[16:218, 51:253, 21:142]
FROM comp_cubed_16 AS img
