FROM gcc:latest

WORKDIR /app

COPY . .

RUN gcc client.c authentication/authentication.c network/network.c session/session.c match/match.c -I./authentication -I./network -I./session -I./match -o client.out


ENTRYPOINT ["./client.out"]

CMD ["server-cont", "8080"]
