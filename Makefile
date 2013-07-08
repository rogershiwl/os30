# comments

# ..\z_tools\make -r bootsectorhelloworld.bin
bootsectorhelloworld.bin : bootsectorhelloworld.nas Makefile
	nask.exe bootsectorhelloworld.nas bootsectorhelloworld.bin bootsectorhelloworld.lst

helloworld.img : bootsectorhelloworld.bin Makefile
	edimg.exe imgin:../z_tools/fdimg0at.tek \
		wbinimg src:bootsectorhelloworld.bin len:512 from:0 to:0 imgout:helloworld.img

img:
	make.exe -r helloworld.img

asm:
	make.exe -r bootsectorhelloworld.bin

run:
	make img
	copy helloworld.img ..\z_tools\qemu\fdimage0.bin
	make.exe -C ../z_tools/qemu

clean:
	del *.bin
	del *.lst
	del *.img

    