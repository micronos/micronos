mkdir "system.img"
cd "system.img"

set root_folder="Temp","Data","Program Data","Program Files","Recovery","System"
for %%a in (%root_folder%) do mkdir %%a

cd "Data"

set data_sub_folder="App Data","Pictures","Music","Videos","Documents","Downloads"
for %%a in (%data_sub_folder%) do mkdir %%a


cd "../System"
set system_sub_folder="Libs","Drivers","Bin","Configs"
for %%a in (%system_sub_folder%) do mkdir %%a
