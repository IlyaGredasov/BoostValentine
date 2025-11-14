FROM debian:12-slim

RUN apt-get update && apt-get install -y

COPY build/boost_valentine .

EXPOSE 8080
CMD ["./boost_valentine"]
