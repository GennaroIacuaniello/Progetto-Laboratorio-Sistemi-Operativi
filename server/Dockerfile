FROM gcc:latest

WORKDIR /app

COPY . .

#If dir data already exists, do nothing
RUN mkdir -p data

RUN gcc server.c authentication/authentication.c network/network.c session/session.c match/match.c -I./authentication -I./network -I./session -I./match -o server.out -lpthread

EXPOSE 8080

ENTRYPOINT ["./server.out"]
