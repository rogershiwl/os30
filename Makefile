# comments

# ..\z_tools\make -r bootsectorhelloworld.bin
bootsectorhelloworld.bin : 2_bootsectorhelloworld.nas Makefile
	../z_tools/nask.exe 2_bootsectorhelloworld.nas bootsectorhelloworld.bin bootsectorhelloworld.lst

helloworld.img : bootsectorhelloworld.bin Makefile
	../z_tools/edimg.exe imgin:../z_tools/fdimg0at.tek \
		wbinimg src:bootsectorhelloworld.bin len:512 from:0 to:0 imgout:helloworld.img

img:
	../z_tools/make.exe -r helloworld.img

asm:
	../z_tools/make.exe -r bootsectorhelloworld.bin

run:
	../z_tools/make.exe img
	copy helloworld.img ..\z_tools\qemu\fdimage0.bin
	../z_tools/make.exe -C ../z_tools/qemu

clean:
	del *.bin
	del *.lst
	del *.img

    