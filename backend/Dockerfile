FROM ruby:2.6-slim-stretch

MAINTAINER devops@employmentinnovations.com

RUN apt-get update -qq --fix-missing \
  && apt-get install -y build-essential libpq-dev libxml2-dev libxslt1-dev cron git \
  && rm -rf /var/lib/apt/lists/* /tmp/*

ARG BUNDLE_GEM__FURY__IO

RUN mkdir -p /app
WORKDIR /app

COPY Gemfile Gemfile.lock ./
RUN gem install bundler && bundle install --jobs 20 --retry 5 --without test

# Set Sinatra to run in production
ENV APP_ENV production

# Copy the main application.
COPY . ./
