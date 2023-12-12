sudo apt install --upgrade zlib1g-dev

git clone https://github.com/agurfinkel/minisat.git
cd minisat
make config prefix=/usr
sudo make install

echo "Successfully installed minisat"