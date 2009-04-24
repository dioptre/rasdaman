#!/usr/bin/ksh
#
# Shell-script that creates C-source code for translating the
# pixmap translation calls used by the wxPixmap-class.
# (C) 2003 Dr. Peter Baumann



# If we don't build to a local file things take forever!
TempSourceFile="/tmp/gtrans_build.x"
# Filenames to store the source under
TransSourceFile="wx_pixmap_translate.c"
TransHeaderFile="wx_pixmap_translate.h"
DitherSourceFile="wx_pixmap_dither.cpp"
DitherHeaderFile="wx_pixmap_dither.h"


# Modes to build translators to / from for
ConvertFromModes="0 1 2 3 5 12 15 24"
ConvertToModes="0 1 2 3 4 5 15 24"

# Print debugging info from each function
OutputDebugInfo=0


# Now machine-dependent setups
type_u8="unsigned char"
type_u16="unsigned short"
type_s16="short"
type_u32="unsigned long"


# Bitorder of source and destination data. 0 = lsb, 1 = msb
# These should be identical.
# palette_fill determines the fill order of the palette (also used as screen format):
# bit 0: fill order, bit 1: colour order. i.e. 0 (0bgr), 1 (bgr0), 2 (0rgb), 3 (rgb0)
# sixin16bpp determines which of the three colour-components is 6 bits long
# For typical Unix-systems use 1, 1, 0
# for NT/LINUX use 0, 0, 2
src_bitorder=1
src_byteorder=1
palette_fill=0

sixin16bpp=1

# These will be variable
#dest_bitorder=1
#dest_byteorder=1




# The following lines should be OK for any system.



# $1, $2, $3 names of colour components. Should always be ordered red, green blue
InternalComponentsToInt() {
  InternRGBToIntL="$1 | ($2<<8) | ($3<<16)"
  InternRGBSToIntS15="($1>>3) | ($2<<2) | ($3<<7)"
  InternRGBLToIntS15="($1>>3) | (($2&0xf8)<<2) | (($3&0xf8)<<7)"
}

# $1, $2, $3 names of colour components. Usually (red, green, blue) or (blue, green, red)
# In case of S this means each colour component is 5 bits (top bits in 8)
InitComponentsToInt() {
  # Has the destination mode the same byteorder as the source (native) machine?
  if [ $src_byteorder -eq $dest_byteorder ]; then
    if [ $(( $palette_fill & 1 )) -eq 0 ]; then
      RGBToIntL="$1 | ($2<<8) | ($3<<16)"
      RGBSToIntS15="($1>>3) | ($2<<2) | ($3<<7)"
      RGBLToIntS15="($1>>3) | (($2&0xf8)<<2) | (($3&0xf8)<<7)"
    else
      RGBToIntL="($1<<8) | ($2<<16) | ($3<<24)"
      RGBSToIntS15="($1>>2) | ($2<<3) | ($3<<8)"
      RGBLToIntS15="(($1&0xf8)>>2) | (($2&0xf8)<<3) | ($3<<8)"
    fi
    
    # 16bpp asymmetry, only depends on colour order (which is determined outside this function)
    if [ $sixin16bpp -eq 0 ]; then
      RGBSToIntS16="($1>>2) | ($2<<3) | ($3<<8)"
      RGBLToIntS16="($1>>2) | (($2&0xf8)<<3) | (($3&0xf8)<<8)"
    elif [ $sixin16bpp -eq 1 ]; then
      RGBSToIntS16="($1>>3) | ($2<<3) | ($3<<8)"
      RGBLToIntS16="($1>>3) | (($2&0xfc)<<3) | (($3&0xf8)<<8)"
    else
      RGBSToIntS16="($1>>3) | ($2<<2) | ($3<<7)"
      RGBLToIntS16="($1>>3) | (($2&0xf8)<<2) | (($3&0xfc)<<8)"
    fi
  else
    if [ $(( $palette_fill & 1 )) -eq 0 ]; then
      RGBToIntL="($1<<24) | ($2<<16) | ($3<<8)"
      RGBSToIntS15="($1<<5) | (($2&0x38)<<10) | ($2>>6) | ($3>>1)"
      RGBLToIntS15="(($1&0xf8)<<5) | (($2&0x38)<<10) | ($2>>6) | (($3&0xf8)>>1)"
    else
      RGBToIntL="($1<<16) | ($2<<8) | $3"
      RGBSToIntS15="($1<<6) | (($2&0x18)<<11) | ($2>>5) | $3"
      RGBLToIntS15="((1&0xf8)<<6) | (($2&0x18)<<11) | ($2>>5) | ($3&0xf8)"
    fi
    
    if [ $sixin16bpp -eq 0 ]; then
      RGBSToIntS16="($1<<6) | (($2&0x18)<<11) | ($2>>5) | $3"
      RGBLToIntS16="(($1&0xfc)<<6) | (($2&0x18)<<11) | ($2>>5) | ($3&0xf8)"
    elif [ $sixin16bpp -eq 1 ]; then
      RGBSToIntS16="($1<<5) | (($2&0x1c)<<11) | ($2>>5) | $3"
      RGBLToIntS16="(($1&0xf8)<<5) | (($2&0x1c)<<11) | ($2>>5) | ($3&0xf8)"
    else
      RGBSToIntS16="($1<<5) | (($2&0x38)<<10) | ($2>>6) | $3"
      RGBLToIntS16="(($1&0xf8)<<5) | (($2&0x38)<<10) | ($2>>6) | ($3&0xfc)"
    fi
  fi
  
}


# $1, $2, $3 = component names of the three colours
InitRGBToInt() {
  if [ $(( $palette_fill & 2 )) -eq 0 ]; then
    WriteRGB24="*dest_ptr++ = $1, *dest_ptr++ = $2, *dest_ptr++ = $3"
    InitComponentsToInt $1 $2 $3
  else
    WriteRGB24="*dest_ptr++ = $3, *dest_ptr++ = $2, *dest_ptr++ = $1"
    InitComponentsToInt $3 $2 $1
  fi
}


# $1 = cast, $2, $3, $4 component names, $5 aux var
InitIntToComponents() {
# Internal representation is constant, RGB starting from bit 0
IntLToRGBL="$2=$1($5&0xff), $3=$1(($5>>8)&0xff), $4=$1(($5>>16)&0xff)"
IntLToRGBS="$2=$1($5&0xf8), $3=$1(($5>>8)&0xf8), $4=$1(($5>>16)&0xf8)"
IntSToRGB="$2=$1(($5<<3)&0xf8), $3=$1(($5>>2)&0xf8), $4=$1(($5>>7)&0xf8)"
}


# $1 = cast. This could contain spaces so make sure to enclose it in quotes!
# $2, $3, $4 = component names of the three colours, $5 = auxiliary variable
InitIntToRGB() {
# Same reason here
  InitIntToComponents "$1" $2 $3 $4 $5
}



# Output functions.

# Initialise the output: $1 = filename
InitOutput () {
  rm -f $1
  CurrentFile=$1
  CurrentIndent=0
  TabChar=$(echo \\011)
  CurrentPrefix=""
}

# Output data _formatted_. Brace checks are primitive but work OK here.
output() {
  if [ "$1" = '}' ] && [ $CurrentIndent -gt 0 ]; then
    CurrentIndent=$(($CurrentIndent-1))
    CurrentPrefix=${CurrentPrefix%$TabChar}
  fi
  echo "$CurrentPrefix$1" >> $CurrentFile
  if [ "$1" = '{' ]; then
    CurrentIndent=$(($CurrentIndent+1))
    CurrentPrefix="$CurrentPrefix$TabChar"
  fi
}


# Writes some information in the header

WriteHeaderComment() {
  output "/*"
  output " *  $CurrentFile"
  output " *  $CurrentDescriptor"
  output " *  Auto-generated $(date)"
  output " *  (C) 2003, Dr. Peter Baumann"
  output " */"
  output ""
}


# Get a pixel.
# $1 = src-ldbpp, $2 = dest-ldbpp, $3 = pixel number

GetPixel() {
  if [ $1 -lt 3 ]; then
    if [ $src_bitorder -eq 0 ]; then
      pixel_value="$(( ($3 * (1<<$1)) & 7))"
    else
      pixel_value="$(( ( ($src_ppc-1-$3) * (1<<$1) ) & 7))"
    fi
    subidx=$(( $3 >> (3-$1) ))
    pixel_value="((src_ptr[subidx + $subidx] >> $pixel_value ) & $(( (1<<$sbpp)-1 )) )"
  else
    if [ $1 -eq 12 ]; then
      pixel_value="((*src_ptr++)>>4)"
    elif [ $1 -eq 24 ]; then
      pixel_value="red=*src_ptr++, green=*src_ptr++, blue=*src_ptr++"
    else
      pixel_value="src_ptr[i + $3]"
    fi
  fi
}


# Get a pixel without the optimizations possible in GetPixel
# ( ((i >> (3-$1)) << (3-$1)) == i ) and translate pixel to RGB
# values. $1 = src-ldbpp, $2 = dest_ldbpp

GetRGBPixel() {
  if [ $1 -lt 3 ]; then
    if [ $src_bitorder -eq 0 ]; then
      pixel_value="( (i*$sbpp) & 7)"
    else
      pixel_value="( ($((8-$sbpp)) - i*$sbpp) & 7)"
    fi
    pixel_value="pixPal=pixmapPalette + ((src_ptr[i >> $(( 3-$1 ))] >> $pixel_value ) & $(( (1<<$sbpp)-1 )) ), red=pixPal->red, green=pixPal->green, blue=pixPal->blue"
  else
    if [ $1 -eq 24 ]; then
      pixel_value="red=*src_ptr++, green=*src_ptr++, blue=*src_ptr++"
    elif [ $1 -eq 12 ]; then
      pixel_value="red=((*src_ptr++)>>4), green=red, blue=red"
    else
      pixel_value="src_ptr[i]"
      if [ $1 -eq 3 ]; then
	pixel_value="pixPal=pixmapPalette + $pixel_value, red=pixPal->red, green=pixPal->green, blue=pixPal->blue"
      # Basically $1 can only be 15
      elif [ $1 -eq 15 ] || [ $1 -eq 4 ]; then
	pixel_value="val=$pixel_value, $IntSToRGB"
      else
	pixel_value="val=$pixel_value, $IntLToRGBL"
      fi
    fi
  fi
}


# Get a pixel and translate it to the destination mode.
# Arguments as for GetPixel
GetPixelTrans() {
  # First create the command to actually get the pixel
  GetPixel $1 $2 $3

  # Now Translate the pixel

  # Source mode not true-colour
  if [ $1 -lt 4 ]; then
    if [ $2 -gt 3 ]; then
      if [ $2 -eq 24 ]; then
        pixel_value="val=ttl[$pixel_value], $IntLToRGBL"
      else
        pixel_value="ttl[$pixel_value]"
      fi
    else
      pixel_value="tt[$pixel_value]"
    fi
  # Source-mode 15bpp
  elif [ $1 -eq 15 ]; then
    # destination mode <= 8bpp
    if [ $2 -lt 4 ]; then
      pixel_value="tt[$pixel_value & 0x7fff]"
    # destination mode 15bpp (==> change bitorder)
    elif [ $2 -eq 4 ]; then
      pixel_value="(val=$pixel_value, $IntSToRGB, $RGBSToIntS16)"
    elif [ $2 -eq 5 ]; then
      pixel_value="(val=$pixel_value, $IntSToRGB, $RGBToIntL)"
    elif [ $2 -eq 15 ]; then
      pixel_value="(val=$pixel_value, $IntSToRGB, $RGBSToIntS15)"
    elif [ $2 -eq 24 ]; then
      pixel_value="(val=$pixel_value, $IntSToRGB)"
    fi
  # Source-mode 12bpp
  elif [ $1 -eq 12 ]; then
    if [ $2 -lt 4 ]; then
      pixel_value="tt[($pixel_value)&0xff]"
    elif [ $2 -eq 4 ]; then
      pixel_value="(red=($pixel_value)&0xf8, green=red, blue=red, $RGBSToIntS16)"
    elif [ $2 -eq 5 ]; then
      pixel_value="(red=$pixel_value, green=red, blue=red, $RGBToIntL)"
    elif [ $2 -eq 15 ]; then
      pixel_value="(red=($pixel_value)&0xf8, green=red, blue=red, $RGBSToIntS15)"
    elif [ $2 -eq 24 ]; then
      pixel_value="(red=$pixel_value, green=red, blue=red)"
    fi
  # Source-mode 32bpp
  elif [ $1 -eq 5 ]; then
    if [ $2 -eq 24 ]; then
      pixel_value="(val=$pixel_value, $IntLToRGBL)"
    else
      # Destination mode 32bpp (==> change bitorder)
      if [ $2 -eq 5 ]; then
        pixel_value="(val=$pixel_value, $IntLToRGBL, $RGBToIntL)"
      else
        if [ $2 -eq 4 ]; then
	  pixel_value="(val=$pixel_value, $IntLToRGBS, $RGBSToIntS16)"
	else
          if [ $2 -eq 15 ]; then
            pixel_value="(val=$pixel_value, $IntLToRGBS, $RGBSToIntS15)"
          else
            pixel_value="tt[(val=$pixel_value, $IntLToRGBS, $InternRGBSToIntS15)]"
          fi
	fi
      fi
    fi
  # Source mode 24bpp
  elif [ $1 -eq 24 ]; then
    if [ $2 -eq 5 ]; then
      pixel_value="($pixel_value, $RGBToIntL)"
    elif [ $2 -ne 24 ]; then
      if [ $2 -eq 4 ]; then
        pixel_value="($pixel_value, $RGBLToIntS16)"
      else
        if [ $2 -lt 4 ]; then
          pixel_value="tt[($pixel_value, $InternRGBLToIntS15)]"
	else
	  pixel_value="($pixel_value, $RGBLToIntS15)"
	fi
      fi
    fi
  fi

  # And finally shift the pixel for non-24bpp destination modes
  if [ $2 -ne 24 ]; then
    if [ $2 -eq 15 ]; then
      usedld=4
    else
      usedld=$2
    fi
    if [ $2 -lt 3 ]; then
      if [ $dest_bitorder -eq 0 ]; then
        pixel_value="($pixel_value << $(( ( $3*(1<<$usedld) ) & 7 )) )"
      else
        pixel_value="($pixel_value << $(( ( ($dest_ppw-1-$3)*(1<<$usedld) ) & 7 )) )"
      fi
    else
      if [ $src_byteorder -eq 0 ]; then
        pixel_value="($pixel_value << $(( ( $3*(1<<$usedld) ) & 31 )) )"
      else
        pixel_value="($pixel_value << $(( ( ($dest_ppw-1-$3)*(1<<$usedld) ) & 31 )) )"
      fi
    fi
  fi
}



# Sets the sbpp and dbpp values according to $1 and $2
SetModeBpp() {
  if [ $1 -lt 6 ]; then
    sbpp=$((1<<$1))
  else
    sbpp=$1
  fi
  if [ $2 -lt 6 ]; then
    dbpp=$((1<<$2))
  else
    dbpp=$2
  fi
}



# Outputs a function prototype signature if the function signature is not empty
OutputFuncSig() {
  if [ "$func_sig" != "" ]; then
    output "$func_sig;"
    output
  fi
}


# Sets the variable func_sig to the signature of the translator function with
# $1 = src-ldbpp, $2 = dest_ldbpp
# if either is larger than 5 it's interpreted as bpp rather than ldbpp

TranslatorSignature() {

  SetModeBpp $1 $2

  if [ $sbpp -le 8 ] && [ $dbpp -eq 15 ]; then
    func_sig=""
    return
  fi

  if [ $src_bitorder -eq $dest_bitorder ] && [ $src_byteorder -eq $dest_byteorder ]; then
    sig_post=""
  else
    sig_post="i"
  fi
  if [ $dbpp -gt 8 ]; then
    if [ $(( palette_fill & 2 )) -eq 0 ]; then
      sig_post=${sig_post}rgb
    else
      sig_post=${sig_post}bgr
    fi
  fi
  func_sig="void wx_pixmap_translate_${sbpp}_to_${dbpp}${sig_post} (const $type_u8 *src, $type_u8 *dest, int width, int height, int srcPitch, int destPitch, const $type_u8 *tt)"
}



# Arguments: $1 = src-ldbpp, $2 = dest-ldbpp

MakeTranslationFunction() {

  TranslatorSignature $1 $2

  if [ "$func_sig" = "" ]; then
    return
  fi

  output "$func_sig"
  output \{

  # Produce translation code for ALL modes because the order of the colour components
  # and the endianness might differ.
  if [ $1 -ne 24 ] || [ $(( $palette_fill & 2 )) -ne 0 ]; then
    need_translator=1
  else
    need_translator=0
  fi
  
  if [ $1 -ne $2 ]; then
    need_translator=1
    if [ $1 -lt 4 ] && [ $2 -eq 15 ]; then
      need_translator=0
    fi
  fi
  
  # Is a translator function necessary?
  if [ $need_translator -ne 0 ]; then

  # Determines what special variables are needed
  needs_val=0
  needs_rgb=0

  # setup variable block
  output "const $type_u8 *src_line = (const $type_u8 *)src;"
  output "$type_u8 *dest_line = dest;"

  case $sbpp in
    1 | 2 | 4 | 8 | 24) type_srcptr=$type_u8 ;;
    12 | 15 | 16) type_srcptr=$type_u16 ;;
    32) type_srcptr=$type_u32 ;;
  esac
  output "register const $type_srcptr *src_ptr;"

  if [ $dbpp -ge 8 ] && [ $dbpp -ne 24 ]; then
    type_destptr=$type_u32;
  else
    type_destptr=$type_u8;
  fi
  output "register $type_destptr *dest_ptr;"

  if [ $1 -gt 3 ] && [ ! $1 -eq 12 ] && [ $2 -gt 3 ]; then
    needs_rgb=1; needs_val=1
  elif [ $sbpp -eq 24 ] || [ $sbpp -eq 32 ] || [ $dbpp -eq 24 ] || [ $dbpp -eq 15 ]; then
    needs_rgb=1; needs_val=1
  fi
  if [ $1 -eq 24 ] && [ $2 -gt 3 ]; then
    needs_val=0
  fi
  if [ $1 -eq 12 ] && [ $dbpp -ge 15 ]; then
    needs_rgb=1;
  fi

  # Cast translation table if ldsrc < 4 and lddest > 3
  if [ $1 -lt 4 ] && [ $2 -gt 3 ]; then
    output "const $type_u32 *ttl = (const $type_u32 *)tt;"
  fi

  output "register int i;"
  output "int j, i_high;"
  if [ $1 -lt 3 ]; then
    output "register int subidx;"
  fi

  # Evaluate pixels per char in source- and pixels per word in dest mode
  src_ppc=$((8/$sbpp))
  if [ $dbpp -eq 15 ] || [ $dbpp -eq 12 ]; then
    dest_ppw=2
  elif [ $dbpp -ge 8 ]; then
    dest_ppw=$((32/$dbpp))
  else
    dest_ppw=$((8/$dbpp))
  fi
  if [ $src_ppc -gt $dest_ppw ]; then
    passes=$(($src_ppc/$dest_ppw)); upper_modulo=$(($src_ppc-1));
  else
    passes=1; upper_modulo=$(($dest_ppw-1));
  fi

  if [ $(($passes*$dest_ppw)) -gt 2 ]; then
    needs_val=1
  fi

  # Trap some very special cases (not important, just kill ``unused variable'' warnings)
  if [ $sbpp -eq 12 ]; then
    if [ $dbpp -eq 15 ] || [ $dbpp -eq 24 ]; then
      needs_val=0
    fi
  fi
  if [ $sbpp -eq 24 ] && [ $dbpp -eq 4 ]; then
    needs_val=0
  fi

  if [ $needs_val -eq 1 ]; then
    if [ $dbpp -lt 8 ] && [ $sbpp -ne 32 ]; then
      output "$type_u8 val;"
    else
      output "$type_u32 val;"
    fi
  fi
  if [ $needs_rgb -eq 1 ]; then
    output "$type_u8 red, green, blue;"
  fi

  output ""
  if [ $OutputDebugInfo -ne 0 ]; then
    output 'printf("Plot '"$sbpp - $dbpp:"' width = %d, height = %d, pitch(src) = %d, pitch(dest) = %d\\n", width, height, srcPitch, destPitch); fflush(stdout);'
  fi
  output "i_high = (width & ~$upper_modulo );"

  # now for the main loop
  output "for (j=0; j<height; j++, src_line += srcPitch, dest_line += destPitch)"
  output \{
  output "src_ptr = (const $type_srcptr *)src_line; dest_ptr = ($type_destptr *)dest_line;"
  output "for (i=0; i<i_high; i+=$(($passes*$dest_ppw)) )"
  output \{

  if [ $1 -lt 3 ]; then
    output "subidx = (i >> $((3-$1)) );"
  fi

  pass=$passes
  while [ $pass -gt 0 ]; do
    if [ $2 -ne 24 ]; then
      output "*dest_ptr ="
    fi
    loop=0
    while [ $loop -lt $dest_ppw ]; do
      GetPixelTrans $1 $2 $(( ($passes - $pass)*$dest_ppw + $loop ))
      if [ $dbpp -eq 24 ]; then
        output "$pixel_value, $WriteRGB24;"
      else
        output "$pixel_value;"
        if [ $loop -ne $((dest_ppw - 1)) ]; then
	  output "*dest_ptr |= "
        fi
      fi
      loop=$(($loop+1))
    done
    if [ $2 -ne 24 ]; then
      output "dest_ptr++;"
    fi
    pass=$(($pass-1))
  done;

  output \}

  # Now pad the remaining bytes for modes with less than 32bpp
  if [ $(($passes*$dest_ppw)) -gt 1 ]; then
    output "if (i < width)"
    output \{
    GetPixelTrans $1 $2 0
    if [ $1 -lt 3 ]; then
      output "subidx = (i >> $((3-$1)) );"
    fi
    if [ $(($passes*$dest_ppw)) -eq 2 ]; then
      if [ $2 -ne 24 ]; then
        output "*dest_ptr++ = $pixel_value;"
      else
        output "$pixel_value, $WriteRGB24;"
      fi
    else
      if [ $2 -ne 24 ]; then
        output "val = $pixel_value;"
      else
        output "$pixel_value, $WriteRGB24;"
      fi
      loop=1
      while [ $loop -lt $(($passes*$dest_ppw - 1)) ]; do
        GetPixelTrans $1 $2 $loop
        output "if (i+$loop < width)"
        output \{
          if [ $2 -ne 24 ]; then
            if [ $(($loop & ($dest_ppw - 1) )) -eq 0 ]; then
              output "*dest_ptr++ = val; val = $pixel_value;"
            else
              output "val |= $pixel_value;"
            fi
          else
            output "$pixel_value, $WriteRGB24;"
          fi
        output \}
        loop=$(($loop+1))
      done
      if [ $2 -ne 24 ]; then
        output "*dest_ptr++ = ($type_destptr)val;"
      fi
    fi
    output \}
  fi

  output \}

  if [ $OutputDebugInfo -ne 0 ]; then
    output 'printf("OK.\\n"); fflush(stdout);'
  fi

  # from 15->15 / 24->24 / 32->32 trap
  fi

  output \}
}



# Signature of a dithering function
# This is realised as wxPixmap member-functions
DithererSignature() {

  SetModeBpp $1 $2

  if [ $src_bitorder -eq $dest_bitorder ] && [ $src_byteorder -eq $dest_byteorder ]; then
    sig_post=""
  else
    sig_post="i"
  fi
  func_sig="void wxPixmap::dither_${sbpp}_to_${dbpp}${sig_post} ($type_u8 *dest, int destPad)"
}




# Ditherer from mode $1 to mode $2

MakeDitheringFunction() {
  # Only make ditherers for displays with <= 8bpp
  if [ $2 -lt 4 ]; then

  DithererSignature $1 $2
  output "$func_sig"

  output \{

  need_idx=0

  output "$type_s16 *errors, *err_ptr;"
  output "$type_s16 red, green, blue, err_red_r, err_green_r, err_blue_r, err_red_d, err_green_d, err_blue_d;"
  output "const $type_u8 *src_line = (const $type_u8 *)data;"
  output "$type_u8 *dest_line = ($type_u8 *)dest;"

  case $1 in
    0 | 1 | 2 | 3 | 24) type_srcptr=$type_u8 ;;
    4 | 12 | 15) type_srcptr=$type_u16 ;;
    5) type_srcptr=$type_u32 ;;
  esac

  output "register const $type_srcptr *src_ptr;"
  output "$type_u8 *dest_ptr;"
  output "$type_u8 r, g, b;"
  output "int i, j, destPitch;"

  output "register wx_permute_cmap *pixPal;"

  if [ $1 -eq 4 ] || [ $1 -eq 15 ]; then
    output "$type_u16 val;"
  elif [ $1 -eq 5 ]; then
    output "$type_u32 val;"
  fi
  if [ $2 -lt 3 ]; then
    output "$type_u8 pixels;"
    output "int shift;"
  fi

  if [ $OutputDebugInfo -ne 0 ]; then
    output "*errorstr << \"Dither $1 to $2\" << endl;"
  fi

  output ""
  output "if ((errors = ($type_s16 *)malloc(3*width*sizeof($type_s16))) == NULL) return;"
  output "memset((void*)errors, 0, 3*width*sizeof($type_s16));"

  # Pixels per char in src / dest mode
  src_ppc=$(( 8 >> $1 ))
  dest_ppc=$(( 8 >> $2 ))

  if [ $dest_bitorder -eq 0 ]; then
    shift_start=0; shift_step=$dbpp; shift_end=8;
  else
    shift_start=$((8-$dbpp)); shift_step=$((-$dbpp)); shift_end=$((0-$dbpp))
  fi

  output "destPitch = ((width*$dbpp + destPad-1) & ~(destPad-1)) >> 3;"

  output "for (j=0; j<height; j++, src_line += pitch, dest_line += destPitch)"
  output \{
  output "src_ptr = (const $type_srcptr *)src_line; dest_ptr = ($type_u8 *)dest_line;"
  output "err_red_r = 0; err_green_r = 0; err_blue_r = 0;"
  output "err_red_d = 0; err_green_d = 0; err_blue_d = 0;"
  if [ $2 -lt 3 ]; then
    output "pixels = 0; shift = $shift_start;"
  fi
  output "for (i=0, err_ptr=errors; i<width; i++, err_ptr += 3)"
  output \{
  GetRGBPixel $1 $2
  output "$pixel_value;"
  output "red += err_red_r + err_ptr[0]; green += err_green_r + err_ptr[1]; blue += err_blue_r + err_ptr[2];"
  output "if (red < 0) r=0; else if (red > 255) r=255; else r=red;"
  output "if (green < 0) g=0; else if (green > 255) g=255; else g=green;"
  output "if (blue < 0) b=0; else if (blue > 255) b=255; else b=blue;"
  output "pixPal = parentPalette + (this->*colour_matcher)(r, g, b);"
  output "red = ($type_s16)r - ($type_s16)(pixPal->red); green = ($type_s16)g - ($type_s16)(pixPal->green); blue = ($type_s16)b - ($type_s16)(pixPal->blue);"
  output "err_red_r = (3*red)/8; err_green_r = (3*green)/8; err_blue_r = (3*blue)/8;"
  output "err_ptr[0] = err_red_r + err_red_d; err_ptr[1] = err_green_r + err_green_d; err_ptr[2] = err_blue_r + err_blue_d;"
  output "err_red_d = red/4; err_green_d = green/4; err_blue_d = blue/4;"
  output "err_red_r = red - err_red_d - err_red_r; err_green_r = green - err_green_d - err_green_r; err_blue_r = blue - err_blue_d - err_blue_r;"

  if [ $2 -eq 3 ]; then
    output "*dest_ptr++ = pixPal->number;"
  else
    output "pixels |= (pixPal->number << shift);"
    output "shift += $shift_step;"
    output "if (shift == $shift_end)"
    output \{
    output "*dest_ptr++ = pixels; pixels = 0; shift = $shift_start;"
    output \}
  fi
  output \}
  if [ $2 -ne 3 ]; then
    output "if (shift != $shift_start) *dest_ptr++ = pixels;"
  fi
  output \}
  output "free(errors);"

  output \}

  # ($2 < 4)
  fi
}




# Customizations
if [ "$1" = "0" ] || [ "$1" = "1" ]; then
  src_bitorder=$1
fi
if [ "$2" = "0" ] || [ "$2" = "1" ]; then
  src_byteorder=$2
fi
if [ "$3" = "0" ] || [ "$3" = "1" ] || [ "$3" = "2" ] || [ "$3" = "3" ]; then
  palette_fill=$3
fi

localdisplay=0
if [ "$4" = "-local" ]; then
  localdisplay=1
fi

echo "Using src_bitorder=$src_bitorder, src_byteorder=$src_byteorder, palette_fill=$palette_fill"
if [ $localdisplay -eq 0 ]; then
  echo "Create translators for different endianness"
else
  echo "Local displays only, one setting."
fi

echo "Creating ditherers for modes $ConvertFromModes --> $ConvertToModes:"

CurrentDescriptor="Dithering functions from/to various depths."

echo "Creating source file $DitherSourceFile ..."

InitOutput $TempSourceFile

WriteHeaderComment

output "// This file must be included from wx_pixmap.cpp"
output ""

# Make translation functions

InternalComponentsToInt red green blue

# We have to init these macros for native and inverted byteorder

dest_bitorder=$src_bitorder; dest_byteorder=$src_byteorder
InitRGBToInt red green blue
InitIntToRGB "($type_s16)" red green blue val

for source_mode in $ConvertFromModes; do
  for dest_mode in $ConvertToModes; do
    MakeDitheringFunction $source_mode $dest_mode
    output ""
  done
done

# Create additional plotters for network displays?
if [ $localdisplay -eq 0 ]; then

  dest_bitorder=$(( $src_bitorder ^ 1 )); dest_byteorder=$(( $src_byteorder ^ 1))
  InitRGBToInt red green blue
  InitIntToRGB "($type_s16)" red green blue val

  for source_mode in $ConvertFromModes; do
    for dest_mode in $ConvertToModes; do
      MakeDitheringFunction $source_mode $dest_mode
      output ""
    done
  done

fi	# localdisplay

mv $TempSourceFile $DitherSourceFile

echo "Creating header file $DitherHeaderFile ..."

InitOutput $TempSourceFile

for source_mode in $ConvertFromModes; do
  for dest_mode in $ConvertToModes; do
    if [ $dest_mode -lt 4 ]; then
      dest_bitorder=$src_bitorder; dest_byteorder=$src_byteorder
      DithererSignature $source_mode $dest_mode
      output "$func_sig;"
      if [ $localdisplay -eq 0 ]; then
        dest_bitorder=$(( $src_bitorder ^ 1 )); dest_byteorder=$(( $src_byteorder ^ 1))
        DithererSignature $source_mode $dest_mode
        output "$func_sig;"
      fi
    fi
  done
done

mv $TempSourceFile $DitherHeaderFile




# Now down to business: create all translation functions as
# C code.


echo "Creating converters for modes $ConvertFromModes --> $ConvertToModes:"

CurrentDescriptor="Translator functions for bitmaps from/to various depths."

echo "Creating source file $TransSourceFile ..."

InitOutput $TempSourceFile

if [ $OutputDebugInfo -ne 0 ]; then
  output "#include <stdio.h>"
fi
output "#include \"$TransHeaderFile\""
output ""

WriteHeaderComment

dest_bitorder=$src_bitorder; dest_byteorder=$src_byteorder
InitRGBToInt red green blue
InitIntToRGB "($type_u8)" red green blue val

for source_mode in $ConvertFromModes; do
  for dest_mode in $ConvertToModes; do
    MakeTranslationFunction $source_mode $dest_mode
    output ""
  done
done

palette_fill=$(( $palette_fill ^ 2 ))

InitRGBToInt red green blue
InitIntToRGB "($type_u8)" red green blue val
  
for source_mode in $ConvertFromModes; do
  for dest_mode in $ConvertToModes; do
    if [ $dest_mode -gt 3 ]; then
      MakeTranslationFunction $source_mode $dest_mode
      output ""
    fi
  done
done

palette_fill=$(( $palette_fill ^ 2 ))

if [ $localdisplay -eq 0 ]; then  
  dest_bitorder=$(( $src_bitorder ^ 1 )); dest_byteorder=$(( $src_byteorder ^ 1 ))
  
  InitRGBToInt red green blue
  InitIntToRGB "($type_u8)" red green blue val

  for source_mode in $ConvertFromModes; do
    for dest_mode in $ConvertToModes; do
      MakeTranslationFunction $source_mode $dest_mode
      output ""
    done
  done

  palette_fill=$(( $palette_fill ^ 2 ))
  
  InitRGBToInt red green blue
  InitIntToRGB "($type_u8)" red green blue val
  
  for source_mode in $ConvertFromModes; do
    for dest_mode in $ConvertToModes; do
      if [ $dest_mode -gt 3 ]; then
        MakeTranslationFunction $source_mode $dest_mode
	output ""
      fi
    done
  done
  
  palette_fill=$(( $palette_fill ^ 2 ))
  
fi	# localdisplay

InitRGBToInt red green blue
InitIntToRGB "($type_u8)" red green blue


mv $TempSourceFile $TransSourceFile


# Then write the header file

echo "Creating header file $TransHeaderFile ..."

InitOutput $TempSourceFile

output "#ifndef _WX_PIXMAP_TRANSLATE_H_"
output "#define _WX_PIXMAP_TRANSLATE_H_"
output ""
output "#ifdef __cplusplus"
output 'extern "C" {'
output "#endif"
output ""

WriteHeaderComment

output "#define WX_PIXMAP_SRC_BITORDER	$src_bitorder"
output "#define WX_PIXMAP_SRC_BYTEORDER	$src_byteorder"
output "#define WX_PIXMAP_PALETTE_FILL	$palette_fill"
output ""


dest_bitorder=$src_bitorder; dest_byteorder=$src_byteorder;
InitRGBToInt "(red)" "(green)" "(blue)"
InitIntToRGB "" "(red)" "(green)" "(blue)" "(val)"

output "/* The following macros are for internal use only */"
output "#define _RGB_TO_PALETTE_LONG(red,green,blue)		($RGBToIntL)"
output "#define _RGBS_TO_PALETTE_SHORT15(red,green,blue)		($RGBSToIntS15)"
output "#define _RGBL_TO_PALETTE_SHORT15(red,green,blue)		($RGBLToIntS15)"
output "#define _RGBS_TO_PALETTE_SHORT16(red,green,blue)		($RGBSToIntS16)"
output "#define _RGBL_TO_PALETTE_SHORT16(red,green,blue)		($RGBLToIntS16)"

if [ $dest_bitorder -eq 0 ]; then dest_bitorder=1; else dest_bitorder=0; fi
if [ $dest_byteorder -eq 0 ]; then dest_byteorder=1; else dest_byteorder=0; fi
InitRGBToInt "(red)" "(green)" "(blue)"
InitIntToRGB "" "(red)" "(green)" "(blue)" "(val)"
output "#define _RGB_TO_PALETTE_LONGi(red,green,blue)		($RGBToIntL)"
output "#define _RGBS_TO_PALETTE_SHORT15i(red,green,blue)		($RGBSToIntS15)"
output "#define _RGBL_TO_PALETTE_SHORT15i(red,green,blue)		($RGBLToIntS15)"
output "#define _RGBS_TO_PALETTE_SHORT16i(red,green,blue)		($RGBSToIntS16)"
output "#define _RGBL_TO_PALETTE_SHORT16i(red,green,blue)		($RGBLToIntS16)"
output ""

output "/* Use these macros if you use wxPixmap class with translations turned on */"
output "#define PALETTE_LONG_TO_RGB(val,red,green,blue)		($IntLToRGBL)"
output "#define PALETTE_SHORT_TO_RGB(val,red,green,blue)	($IntSToRGB)"
output "#define RGBS_TO_PALETTE_SHORT(red,green,blue)		(((red)>>3) | ((green)<<2) | ((blue)<<7))"
output "#define RGBL_TO_PALETTE_SHORT(red,green,blue)		(((red)>>3) | (((green)&0xf8)<<2) | (((blue)&0xf8)<<7))"
output "#define RGB_TO_PALETTE_LONG(red,green,blue)		((red) | ((green)<<8) | ((blue)<<16))"
output ""

for source_mode in $ConvertFromModes; do
  for dest_mode in $ConvertToModes; do
    dest_bitorder=$src_bitorder; dest_byteorder=$src_byteorder
    TranslatorSignature $source_mode $dest_mode
    OutputFuncSig
    if [ $dest_mode -gt 3 ]; then
      palette_fill=$(( $palette_fill ^ 2 ))
      TranslatorSignature $source_mode $dest_mode
      OutputFuncSig
      palette_fill=$(( $palette_fill ^ 2 ))
    fi
    if [ $localdisplay -eq 0 ]; then
      dest_bitorder=$(( src_bitorder ^ 1 )); dest_byteorder=$(( src_byteorder ^ 1 ))
      TranslatorSignature $source_mode $dest_mode
      OutputFuncSig
      if [ $dest_mode -gt 3 ]; then
        palette_fill=$(( $palette_fill ^ 2 ))
	TranslatorSignature $source_mode $dest_mode
	OutputFuncSig
        palette_fill=$(( $palette_fill ^ 2 ))
      fi
    fi
  done
  output ""
done

output "#ifdef __cplusplus"
output "}"
output "#endif"
output ""

output "#endif"

mv $TempSourceFile $TransHeaderFile
