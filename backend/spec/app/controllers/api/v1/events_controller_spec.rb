require 'spec_helper'

describe Api::V1::EventsController, type: :controller do
  let(:headers) { headers_request }
  let!(:events) { create_list(:event, 15) }

  describe 'GET /' do
    context 'authentication' do
      before { get '/', {}, headers }

      it_behaves_like 'authentication request'
    end

    context 'valid request' do
      before { get '/', {}, headers }

      it 'should return list of events' do
        expect(response_status).to eq 200
        expect(response_body[:data][:items].count).to eq 15
      end
    end
  end
end
