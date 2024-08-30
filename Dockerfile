# Use uma imagem base oficial do Ubuntu
FROM ubuntu:latest

# Atualize os repositórios e instale dependências
RUN apt-get update && \
    apt-get install -y openmpi-bin openmpi-doc libopenmpi-dev openssh-server sudo nano


# Defina o diretório de trabalho
WORKDIR /mpi

# Gera chaves SSH e configura
RUN mkdir /var/run/sshd
RUN echo 'root:password' | chpasswd
RUN ssh-keygen -t rsa -f /root/.ssh/id_rsa -q -N ""
RUN cat /root/.ssh/id_rsa.pub >> /root/.ssh/authorized_keys
RUN chmod 600 /root/.ssh/authorized_keys


# Copie o código MPI para o contêiner
COPY file_mpi.c /mpi/file_mpi.c

# Compile o programa MPI
RUN mpicc -o /mpi/file_mpi /mpi/file_mpi.c


# Configura o SSH para aceitar conexões
RUN echo "StrictHostKeyChecking no" >> /etc/ssh/ssh_config

# Exposição da porta SSH
EXPOSE 22


# Defina o ponto de entrada padrão
# Inicie o SSH no Dockerfile
CMD ["/usr/sbin/sshd", "-D"]

