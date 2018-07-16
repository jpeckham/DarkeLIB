Vagrant.configure("2") do |config|
	config.vm.box = "bento/ubuntu-14.04" #ubunutu box that supports hyper v provider
	config.vm.provider "hyperv" do |h|
		h.differencing_disk = true # Faster cloning and uses less disk space
	end
			config.vm.synced_folder '.', '/vagrant', {
			type: 'smb', mount_options: ['vers=3.0'], #Ensures that hyper v can connect to SMB using SMB2
			smb_username: ENV['VAGRANT_SMB_USERNAME'],
			smb_password: ENV['VAGRANT_SMB_PASSWORD']
		}
	config.vm.provision "shell", inline: <<-SHELL
		sudo apt-get update -y
		sudo apt-get install -y autoconf automake bison libbison-dev
		sudo apt-get install -y build-essential gcc g++
		sudo apt-get install -y git
		sudo apt-get install -y libevent-dev
		sudo apt-get install -y libmysqlclient-dev libsqlite3-dev libpq-dev libz-dev libssl-dev libpcre3-dev
		
	SHELL
	config.vm.post_up_message = "Your VM is ready for use."
end
